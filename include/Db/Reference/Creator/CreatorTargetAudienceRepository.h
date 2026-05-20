#pragma once

#include <vector>
#include "Db/DbManager.h"

class CreatorTargetAudienceRepository {

private:
    DbManager& dbManager;

public:
    CreatorTargetAudienceRepository(DbManager& db);

    void insert(int creatorId, int audId);

    std::vector<int> getByCreatorId(int creatorId);

    void softDelete(int creatorId, int audId);
};