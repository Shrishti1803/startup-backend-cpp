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

//Transaction
#include "../Utils/TransactionGuard.h"

class BrandService {

private:

    DbManager& dbManager;
    BrandAggregateRepository& brandAggregateRepo;
    BrandRepository& brandRepo;

public:

    BrandService(
        DbManager& dbManager,
        BrandAggregateRepository& brandAggregateRepo,
        BrandRepository& brandRepo
    );

    // Core operations
    void createBrand(const Session& session, const BrandAggregate& data);

    void updateBrand(const Session& session, int brandId, const BrandAggregate& data);

    void deleteBrand(const Session& session, int brandId);

    BrandAggregate getBrandById(const Session& session, int brandId);
};