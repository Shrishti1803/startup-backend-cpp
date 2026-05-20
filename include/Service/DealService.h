#pragma once

#include "Db/Deal/DealRepository.h"
#include "Db/Deal/DealQueryRepository.h"
#include "Db/DbManager.h"

#include "Auth/RBAC.h"
#include "Auth/AuthService.h"
#include "Db/Brand/BrandRepository.h"
#include "Db/Creator/CreatorRepository.h"

#include "Utils/TransactionGuard.h"

// Pagination Response
struct DealPage {
    std::vector<DealView> data;
    int totalCount;
    int currentPage;
    int totalPages;
};

class DealService {

private:
    DbManager& dbManager;
    DealRepository& dealRepo;
    DealQueryRepository& dealQueryRepo;
    BrandRepository& brandRepo;
    CreatorRepository& creatorRepo;


public:

    DealService(
        DbManager& dbManager,
        DealRepository& dealRepo,
        DealQueryRepository& dealQueryRepo,
        BrandRepository& brandRepo,
        CreatorRepository& creatorRepo
    );

    // CRUD
    int createDeal(const Auth::Session& session, const Deal& deal);
    void updateDeal(const Auth::Session& session, int dealId, const DealUpdate& updateData);
    void deleteDeal(const Auth::Session& session, int dealId);
    Deal getDealById(const Auth::Session& session, int dealId);

    // VIEW (Filter + Sort + Pagination)
    DealPage getDealsView(
        const Auth::Session& session,
        const std::string& brandName = "",
        const std::string& creatorName = "",
        const std::string& sortBy = "date",
        const std::string& sortOrder = "DESC",
        int page = 1,
        int limit = 10
    );
};