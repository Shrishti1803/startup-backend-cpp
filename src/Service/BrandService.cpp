#include "Service/BrandService.h"
#include "Utils/Logger.h"
#include <cppconn/prepared_statement.h>
#include <stdexcept>
#include <memory>
using namespace Auth;

BrandService::BrandService(
    DbManager& dbManager,
    BrandAggregateRepository& brandAggregateRepo,
    BrandRepository& brandRepo,
    BrandQueryRepository& brandQueryRepo,
    CreatorQueryRepository& creatorQueryRepo,
    BrandGenreRepository& brandGenreRepo,
    BrandTargetAudienceRepository& brandTargetRepo
)
    : dbManager(dbManager),
      brandAggregateRepo(brandAggregateRepo),
      brandRepo(brandRepo),
      brandQueryRepo(brandQueryRepo),
      creatorQueryRepo(creatorQueryRepo),
      brandGenreRepo(brandGenreRepo),
      brandTargetRepo(brandTargetRepo)
{
}


// CREATE
int BrandService::createBrand(
    const Session& session,
    const BrandAggregate& data
)
{
    if (!RBAC::canCreate(session)) {
        throw std::runtime_error("Permission denied: cannot create brand");
    }

    sql::Connection* conn = dbManager.getConnection();

    if (!conn) {
        Logger::db()->error("DB connection failed during brand creation");
        throw std::runtime_error("Database connection not available");
    }

    TransactionGuard tx(conn);

    try {
        int brandId = brandAggregateRepo.create(conn, session.user_id, data);

        tx.commit();

        Logger::db()->info("Brand created successfully with ID {} by user {}", brandId, session.user_id);

        return brandId;
    }
    catch (const std::exception& e) {
        Logger::db()->error("Brand creation failed for user {}: {}", session.user_id, e.what());
        throw;
    }
}


// UPDATE
void BrandService::updateBrand(
    const Session& session,
    int brandId,
    const BrandAggregate& data
)
{
    if (!RBAC::canUpdate(session)) {
        throw std::runtime_error("Permission denied: cannot update brand");
    }

    Logger::db()->info("User {} started updating brand {}", session.user_id, brandId);

    sql::Connection* conn = dbManager.getConnection();

    if (!conn) {
        Logger::db()->error("DB connection failed during brand update");
        throw std::runtime_error("Database connection not available");
    }

    TransactionGuard tx(conn);

    try {
        brandAggregateRepo.update(conn, session.user_id, brandId, data);

        tx.commit();

        Logger::db()->info("Brand {} updated successfully by user {}", brandId, session.user_id);
    }
    catch (const std::exception& e) {
        Logger::db()->error("Brand update failed for brand {} by user {}: {}", brandId, session.user_id, e.what());
        throw;
    }
}


// DELETE
void BrandService::deleteBrand(
    const Session& session,
    int brandId
)
{
    if (!RBAC::canDelete(session)) {
        throw std::runtime_error("Permission denied: cannot delete brand");
    }

    Logger::db()->info("User {} started deleting brand {}", session.user_id, brandId);

    sql::Connection* conn = dbManager.getConnection();

    if (!conn) {
        Logger::db()->error("DB connection failed during brand delete");
        throw std::runtime_error("Database connection not available");
    }

    TransactionGuard tx(conn);

    try {
        brandAggregateRepo.softDelete(conn, session.user_id, brandId);

        tx.commit();

        Logger::db()->info("Brand {} deleted successfully by user {}", brandId, session.user_id);
    }
    catch (const std::exception& e) {
        Logger::db()->error("Brand delete failed for brand {} by user {}: {}", brandId, session.user_id, e.what());
        throw;
    }
}

void BrandService::deletePartial(
    const Session& session,
    int brandId,
    const BrandAggregate& data
)
{
    if (!RBAC::canDelete(session)) {
        throw std::runtime_error("Permission denied");
    }

    sql::Connection* conn = dbManager.getConnection();

    if (!conn) {
        throw std::runtime_error("DB connection failed");
    }

    TransactionGuard tx(conn);

    try {
        brandAggregateRepo.deletePartial(
            conn,
            session.user_id,
            brandId,
            data
        );

        tx.commit();
    }
    catch (...) {
        throw;
    }
}


// READ
BrandAggregate BrandService::getBrandById(
    const Session& session,
    int brandId
)
{
    if (!RBAC::canView(session)) {
        throw std::runtime_error("Permission denied: cannot view brand");
    }

    Logger::db()->info("User {} fetching brand {}", session.user_id, brandId);

    return brandAggregateRepo.getById(brandId);
}

SearchResult BrandService::searchAll(
    [[maybe_unused]] const Session& session,
    const std::string& query
)
{
    SearchResult result;

    if (query.empty()) return result;

    sql::Connection* conn = dbManager.getConnection();

    if (!conn) {
        throw std::runtime_error("Database connection not available");
    }

    result.brands = brandQueryRepo.searchBrandNames(query);
    result.creators = creatorQueryRepo.searchCreators(conn, query);

    return result;
}

// 🔥 MAIN FUNCTION (FINAL FIXED)
BrandPage BrandService::getBrandsView(
    const Session& session,
    const std::string& searchQuery,
    const std::vector<int>& genreIds,
    const std::string& sortBy,
    const std::string& sortOrder,
    int page,
    int limit
) {
    // 🔐 RBAC
    if (!RBAC::canView(session)) {
        throw std::runtime_error("Unauthorized access");
    }

    // 🛑 Safety guards (VERY IMPORTANT)
    if (page <= 0) page = 1;
    if (limit <= 0) limit = 10;

    int offset = (page - 1) * limit;

    // 📡 DB connection
    sql::Connection* conn = dbManager.getConnection();
    if (!conn) {
        throw std::runtime_error("DB connection failed");
    }

    // 📊 Fetch data (AUTO handles search/filter/sort)
    std::vector<BrandResult> brands =
        brandQueryRepo.getBrands(
            searchQuery,   // "" → ignored
            genreIds,      // {} → ignored
            sortBy,
            sortOrder,
            limit,
            offset
        );

    // 🔢 Count for pagination
    int totalCount =
        brandQueryRepo.getBrandCount(
            searchQuery,
            genreIds
        );

    // 📦 Response
    BrandPage result;
    result.data = brands;
    result.totalCount = totalCount;
    result.currentPage = page;
    result.totalPages = (totalCount + limit - 1) / limit;

    return result;
}
void BrandService::addGenreToBrand(
    const Session& session,
    int brandId,
    int genreId
)
{
    if (!RBAC::canUpdate(session)) {
        throw std::runtime_error("Permission denied: cannot modify brand");
    }

    brandGenreRepo.insert(brandId, genreId);
}

void BrandService::removeGenreFromBrand(
    const Auth::Session& session,
    int brandId,
    int genreId
)
{
    if (!RBAC::canUpdate(session)) {
        throw std::runtime_error("Permission denied: cannot modify brand");
    }

    brandGenreRepo.softDelete(brandId, genreId);
}
void BrandService::addAudienceToBrand(
    const Auth::Session& session,
    int brandId,
    int audId
)
{
    if (!RBAC::canUpdate(session)) {
        throw std::runtime_error("Permission denied: cannot modify brand");
    }

    brandTargetRepo.insert(brandId, audId);
}

void BrandService::removeAudienceFromBrand(
    const Auth::Session& session,
    int brandId,
    int audId
)
{
    if (!RBAC::canUpdate(session)) {
        throw std::runtime_error("Permission denied: cannot modify brand");
    }

    brandTargetRepo.softDelete(brandId, audId);
}