/*
 * BrandService
 *
 * Service layer responsible for handling business logic related to Brand operations.
 * This layer acts as an intermediary between the application/UI layer and the
 * repository layer.
 *
 * Responsibilities:
 *  - Perform Role Based Access Control (RBAC) checks before executing operations
 *  - Manage database transaction boundaries using TransactionGuard (RAII)
 *  - Coordinate calls to BrandAggregateRepository for multi-table operations
 *
 * The actual database persistence is handled by the repository layer, while
 * this service ensures that operations are executed safely, atomically,
 * and only by authorized users.
*/

// NOTE:
// Transaction management is handled in this layer to ensure that
// complex multi-table operations either complete fully or roll back
// safely in case of any failure.
#include "Service/BrandService.h"
#include <stdexcept>


BrandService::BrandService(
    DbManager& dbManager,
    BrandAggregateRepository& brandAggregateRepo,
    BrandRepository& brandRepo
)
    : dbManager(dbManager),
      brandAggregateRepo(brandAggregateRepo),
      brandRepo(brandRepo)
{
}


void BrandService::createBrand(
    const Session& session,
    const BrandAggregate& data
)
{
    if (!RBAC::canCreate(session)) {
        throw std::runtime_error("Permission denied: cannot create brand");
    }

    sql::Connection* conn = dbManager.getConnection();

    if (!conn) {
        throw std::runtime_error("Database connection not available");
    }

    TransactionGuard tx(conn);

    brandAggregateRepo.create(conn, session.user_id, data);

    tx.commit();
}


void BrandService::updateBrand(
    const Session& session,
    int brandId,
    const BrandAggregate& data
)
{
    if (!RBAC::canUpdate(session)) {
        throw std::runtime_error("Permission denied: cannot update brand");
    }

    sql::Connection* conn = dbManager.getConnection();

    if (!conn) {
        throw std::runtime_error("Database connection not available");
    }

    TransactionGuard tx(conn);

    brandAggregateRepo.update(conn, session.user_id, brandId, data);

    tx.commit();
}


void BrandService::deleteBrand(
    const Session& session,
    int brandId
)
{
    if (!RBAC::canDelete(session)) {
        throw std::runtime_error("Permission denied: cannot delete brand");
    }

    sql::Connection* conn = dbManager.getConnection();

    if (!conn) {
        throw std::runtime_error("Database connection not available");
    }

    TransactionGuard tx(conn);

    brandAggregateRepo.softDelete(conn, session.user_id, brandId);

    tx.commit();
}


BrandAggregate BrandService::getBrandById(
    const Session& session,
    int brandId
)
{
    if (!RBAC::canView(session)) {
        throw std::runtime_error("Permission denied: cannot view brand");
    }

    return brandAggregateRepo.getById(brandId);
}