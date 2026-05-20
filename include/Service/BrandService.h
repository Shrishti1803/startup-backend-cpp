#pragma once

// DB
#include "Db/Brand/BrandAggregateRepository.h"
#include "../Db/Brand/BrandRepository.h"
#include "../Db/DbManager.h"

// Models
#include "../Models/Brand/Brand.h"

// Utils
#include "../Utils/ActivityDetails.h"

// Auth
#include "../Auth/RBAC.h"
#include "../Auth/AuthService.h"

// Transaction
#include "../Utils/TransactionGuard.h"

// Query
#include "Db/Brand/BrandQueryRepository.h"
#include "Db/Creator/CreatorQueryRepository.h"

#include "Db/Reference/Brand/BrandGenreRepository.h"
#include "Db/Reference/Brand/BrandTargetAudienceRepository.h"

struct SearchResult {
    std::vector<BrandResult> brands;
    std::vector<CreatorResult> creators;
};

// Paginated Response
struct BrandPage {
    std::vector<BrandResult> data;
    int totalCount;
    int currentPage;
    int totalPages;
};

class BrandService {

private:
    DbManager& dbManager;
    BrandAggregateRepository& brandAggregateRepo;
    BrandRepository& brandRepo;
    BrandQueryRepository& brandQueryRepo;
    CreatorQueryRepository& creatorQueryRepo;
    BrandGenreRepository& brandGenreRepo;
    BrandTargetAudienceRepository& brandTargetRepo;

public:

    BrandService(
        DbManager& dbManager,
        BrandAggregateRepository& brandAggregateRepo,
        BrandRepository& brandRepo,
        BrandQueryRepository& brandQueryRepo,
        CreatorQueryRepository& creatorQueryRepo,
        BrandGenreRepository& brandGenreRepo,
        BrandTargetAudienceRepository& brandTargetRepo
    );

    // Core operations
    int createBrand(
        const Auth::Session& session,
        const BrandAggregate& data
    );

    void updateBrand(
        const Auth::Session& session,
        int brandId,
        const BrandAggregate& data
    );

    void deleteBrand(
        const Auth::Session& session,
        int brandId
    );

    BrandAggregate getBrandById(
        const Auth::Session& session,
        int brandId
    );

    //  Global search
    SearchResult searchAll(
        const Auth::Session& session,
        const std::string& query
    );

    //  MAIN FEATURE: Search + Filter + Sort + Pagination (ALL OPTIONAL)
    BrandPage getBrandsView(
        const Auth::Session& session,
        const std::string& searchQuery = "",
        const std::vector<int>& genreIds = {},
        const std::string& sortBy = "name",
        const std::string& sortOrder = "ASC",
        int page = 1,
        int limit = 10
    );
    void addGenreToBrand(
        const Auth::Session& session,
        int brandId,
        int genreId
    );
    void removeGenreFromBrand(
        const Auth::Session& session,
        int brandId,
        int genreId
    );
    void addAudienceToBrand(
        const Auth::Session& session,
        int brandId,
        int audId
    );
    void removeAudienceFromBrand(
        const Auth::Session& session,
        int brandId,
        int audId
    );
    void deletePartial(
        const Auth::Session& session,
        int brandId,
        const BrandAggregate& data
    );
};