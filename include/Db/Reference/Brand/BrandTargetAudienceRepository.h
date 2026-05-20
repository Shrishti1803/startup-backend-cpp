#pragma once

#include <vector>
#include "Db/DbManager.h"
#include "Models/References/TargetAudience.h"

class BrandTargetAudienceRepository {

private:
    DbManager& dbManager;

public:
    BrandTargetAudienceRepository(DbManager& db);

    // CREATE
    void insert(int brandId, int audienceId);

    // READ (JOIN)
    std::vector<TargetAudience> getByBrandId(int brandId);

    // DELETE (soft)
    void softDelete(int brandId, int audienceId);
};