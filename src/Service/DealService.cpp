#include "Service/DealService.h"
#include <stdexcept>

using namespace Auth;
// 🔹 Constructor
DealService::DealService(
    DbManager& dbManager,
    DealRepository& dealRepo,
    DealQueryRepository& dealQueryRepo,
    BrandRepository& brandRepo,
    CreatorRepository& creatorRepo
)
    : dbManager(dbManager),
      dealRepo(dealRepo),
      dealQueryRepo(dealQueryRepo),
      brandRepo(brandRepo),
      creatorRepo(creatorRepo)
{
}


// 🔹 CREATE
int DealService::createDeal(
    const Session& session,
    const Deal& deal
)
{
    // 🔐 RBAC
    if (!RBAC::canCreate(session)) {
        throw std::runtime_error("Permission denied: cannot create deal");
    }

    sql::Connection* conn = dbManager.getConnection();
    if (!conn) {
        throw std::runtime_error("DB connection failed");
    }

    // =========================
    // 🧪 BASIC VALIDATION
    // =========================

    if (deal.brand_id <= 0) {
        throw std::runtime_error("Invalid brand ID");
    }

    if (deal.creator_id <= 0) {
        throw std::runtime_error("Invalid creator ID");
    }
    // =========================
    // 🧠 EXISTENCE CHECKS
    // =========================

    if (!brandRepo.existsById(conn, deal.brand_id)) {
        throw std::runtime_error("Brand does not exist");
    }

    if (!creatorRepo.existsById(conn, deal.creator_id)) {
        throw std::runtime_error("Creator does not exist");
    }

    // =========================
    // 🔄 TRANSACTION
    // =========================

    TransactionGuard tx(conn);

    try {
        int dealId = dealRepo.create(conn, deal);

        tx.commit();

        return dealId;
    }
    catch (const std::exception& e) {
        throw;
    }
}


// 🔹 UPDATE
void DealService::updateDeal(
    const Session& session,
    int dealId,
    const DealUpdate& updateData
)
{
    if (!RBAC::canUpdate(session)) {
        throw std::runtime_error("Permission denied: cannot update deal");
    }
    if (dealId <= 0) {
        throw std::runtime_error("Invalid deal ID");
    }

    sql::Connection* conn = dbManager.getConnection();
    if (!conn) throw std::runtime_error("DB connection failed");

    if (!dealRepo.existsById(conn, dealId)) {
        throw std::runtime_error("Deal does not exist");
    }
    Deal existingDeal =
        dealRepo.getById(conn, dealId);

    bool lead =
        updateData.is_lead.has_value()
        ? *updateData.is_lead
        : existingDeal.is_lead;

    bool connected =
        updateData.is_connected.has_value()
        ? *updateData.is_connected
        : existingDeal.is_connected;

    bool dealDone =
        updateData.is_deal_done.has_value()
        ? *updateData.is_deal_done
        : existingDeal.is_deal_done;

    bool postUploaded =
        updateData.post_uploaded.has_value()
        ? *updateData.post_uploaded
        : existingDeal.post_uploaded;

    bool paymentCleared =
        updateData.payment_cleared.has_value()
        ? *updateData.payment_cleared
        : existingDeal.payment_cleared;

    if (connected && !lead)
        throw std::runtime_error(
            "Must be a lead before connecting"
        );

    if (dealDone && !connected)
        throw std::runtime_error(
            "Must be connected before deal"
        );

    if (postUploaded && !dealDone)
        throw std::runtime_error(
            "Deal must be done before posting"
        );

    if (paymentCleared && !postUploaded)
        throw std::runtime_error(
            "Post must be uploaded before payment"
        );
    TransactionGuard tx(conn);

    try {
        dealRepo.update(conn, dealId, updateData);
        tx.commit();
    } catch (...) {
        throw;
    }
}


// 🔹 DELETE (Soft Delete)
void DealService::deleteDeal(
    const Session& session,
    int dealId
)
{
    if (!RBAC::canDelete(session)) {
        throw std::runtime_error("Permission denied: cannot delete deal");
    }

    sql::Connection* conn = dbManager.getConnection();
    if (!conn) throw std::runtime_error("DB connection failed");

    TransactionGuard tx(conn);

    try {
        dealRepo.softDelete(conn, dealId);
        tx.commit();
    } catch (...) {
        throw;
    }
}


// 🔹 READ
Deal DealService::getDealById(
    const Session& session,
    int dealId
)
{
    if (!RBAC::canView(session)) {
        throw std::runtime_error("Permission denied: cannot view deal");
    }

    sql::Connection* conn = dbManager.getConnection();
    if (!conn) throw std::runtime_error("DB connection failed");

    return dealRepo.getById(conn, dealId);
}


// 🔥 VIEW (FILTER + SORT + PAGINATION)
DealPage DealService::getDealsView(
    const Session& session,
    const std::string& brandName,
    const std::string& creatorName,
    const std::string& sortBy,
    const std::string& sortOrder,
    int page,
    int limit
)
{
    if (!RBAC::canView(session)) {
        throw std::runtime_error("Permission denied: cannot view deals");
    }

    // safety
    if (page <= 0) page = 1;
    if (limit <= 0) limit = 10;

    int offset = (page - 1) * limit;

    sql::Connection* conn = dbManager.getConnection();
    if (!conn) throw std::runtime_error("DB connection failed");

    // 📊 data
    std::vector<DealView> deals =
        dealQueryRepo.getDeals(
            conn,
            brandName,
            creatorName,
            sortBy,
            sortOrder,
            limit,
            offset
        );

    // 🔢 count
    int totalCount =
        dealQueryRepo.getDealCount(
            conn,
            brandName,
            creatorName
        );

    // 📦 response
    DealPage result;
    result.data = deals;
    result.totalCount = totalCount;
    result.currentPage = page;
    result.totalPages = (totalCount + limit - 1) / limit;

    return result;
}