#include "Service/CreatorService.h"
#include "Utils/Logger.h"

#include <stdexcept>
using namespace Auth;

// 🔥 FIXED CONSTRUCTOR (IMPORTANT)
CreatorService::CreatorService(
    DbManager& dbManager,
    CreatorAggregateRepository& creatorAggregateRepo,
    CreatorRepository& creatorRepo,
    CreatorQueryRepository& creatorQueryRepo
)
    : dbManager(dbManager),
      creatorAggregateRepo(creatorAggregateRepo),
      creatorRepo(creatorRepo),
      creatorQueryRepo(creatorQueryRepo)
{
}


// CREATE
int CreatorService::createCreator(
    const Session& session,
    const CreatorAggregate& data
)
{
    if (!RBAC::canCreate(session)) {
        throw std::runtime_error("Permission denied: cannot create creator");
    }

    Logger::db()->info("User {} started creating creator", session.user_id);

    sql::Connection* conn = dbManager.getConnection();

    if (!conn) {
        Logger::db()->error("DB connection failed during creator creation");
        throw std::runtime_error("Database connection not available");
    }

    TransactionGuard tx(conn);

    try {
        int creatorId = creatorAggregateRepo.create(
            conn,
            session.user_id,
            data
        );

        tx.commit();

        Logger::db()->info(
            "Creator created successfully with ID {} by user {}",
            creatorId,
            session.user_id
        );

        return creatorId;
    }
    catch (const std::exception& e) {
        Logger::db()->error(
            "Creator creation failed for user {}: {}",
            session.user_id,
            e.what()
        );
        throw;
    }
}


// UPDATE
void CreatorService::updateCreator(
    const Session& session,
    int creatorId,
    const CreatorAggregate& data
)
{
    if (!RBAC::canUpdate(session)) {
        throw std::runtime_error("Permission denied: cannot update creator");
    }

    Logger::db()->info(
        "User {} started updating creator {}",
        session.user_id,
        creatorId
    );

    sql::Connection* conn = dbManager.getConnection();

    if (!conn) {
        Logger::db()->error("DB connection failed during creator update");
        throw std::runtime_error("Database connection not available");
    }

    TransactionGuard tx(conn);

    try {
        creatorAggregateRepo.update(
            conn,
            session.user_id,
            creatorId,
            data
        );

        tx.commit();

        Logger::db()->info(
            "Creator {} updated successfully by user {}",
            creatorId,
            session.user_id
        );
    }
    catch (const std::exception& e) {
        Logger::db()->error(
            "Creator update failed for creator {} by user {}: {}",
            creatorId,
            session.user_id,
            e.what()
        );
        throw;
    }
}


// DELETE
void CreatorService::deleteCreator(
    const Session& session,
    int creatorId
)
{
    if (!RBAC::canDelete(session)) {
        throw std::runtime_error("Permission denied: cannot delete creator");
    }

    Logger::db()->info(
        "User {} started deleting creator {}",
        session.user_id,
        creatorId
    );

    sql::Connection* conn = dbManager.getConnection();

    if (!conn) {
        Logger::db()->error("DB connection failed during creator delete");
        throw std::runtime_error("Database connection not available");
    }

    TransactionGuard tx(conn);

    try {
        creatorAggregateRepo.softDelete(
            conn,
            session.user_id,
            creatorId
        );

        tx.commit();

        Logger::db()->info(
            "Creator {} deleted successfully by user {}",
            creatorId,
            session.user_id
        );
    }
    catch (const std::exception& e) {
        Logger::db()->error(
            "Creator delete failed for creator {} by user {}: {}",
            creatorId,
            session.user_id,
            e.what()
        );
        throw;
    }
}


// READ
CreatorAggregate CreatorService::getCreatorById(
    const Session& session,
    int creatorId
)
{
    if (!RBAC::canView(session)) {
        throw std::runtime_error("Permission denied: cannot view creator");
    }

    Logger::db()->info(
        "User {} fetching creator {}",
        session.user_id,
        creatorId
    );

    return creatorAggregateRepo.getById(creatorId);
}


// 🔥 MAIN VIEW FUNCTION
CreatorPage CreatorService::getCreatorsView(
    const Session& session,
    const std::string& searchQuery,
    int minAudienceSize,
    int maxAudienceSize,
    std::vector<int> nicheIds,
    const std::string& sortBy,
    const std::string& sortOrder,
    int page,
    int limit
)
{
    if (!RBAC::canView(session)) {
        throw std::runtime_error("Permission denied: cannot view creators");
    }

    // safety
    if (page <= 0) page = 1;
    if (limit <= 0) limit = 10;

    int offset = (page - 1) * limit;

    sql::Connection* conn = dbManager.getConnection();
    if (!conn) {
        throw std::runtime_error("Database connection not available");
    }

    std::vector<CreatorResult> creators =
        creatorQueryRepo.getCreators(
            conn,
            searchQuery,
            nicheIds,
            minAudienceSize,
            maxAudienceSize,
            sortBy,
            sortOrder,
            limit,
            offset
        );

    int totalCount =
        creatorQueryRepo.getCreatorCount(
            conn,
            searchQuery,
            nicheIds,
            minAudienceSize,
            maxAudienceSize
        );

    CreatorPage result;
    result.data = creators;
    result.totalCount = totalCount;
    result.currentPage = page;
    result.totalPages = (totalCount + limit - 1) / limit;

    return result;
}

std::vector<Creator> CreatorService::getAllCreators(
    const Session& session
)
{
    if (!RBAC::canView(session))
    {
        throw std::runtime_error(
            "Permission denied: cannot view creators"
        );
    }

    Logger::db()->info(
        "User {} fetching all creators",
        session.user_id
    );

    return creatorRepo.getAll();
}

void CreatorService::deletePartial(
    const Session& session,
    int creatorId,
    const CreatorAggregate& data
)
{
    if (!RBAC::canDelete(session))
    {
        throw std::runtime_error(
            "Permission denied"
        );
    }

    sql::Connection* conn =
        dbManager.getConnection();

    if (!conn)
    {
        throw std::runtime_error(
            "DB connection failed"
        );
    }

    TransactionGuard tx(conn);

    try
    {
        creatorAggregateRepo.deletePartial(
            conn,
            session.user_id,
            creatorId,
            data
        );

        tx.commit();
    }
    catch (...)
    {
        throw;
    }
}