#pragma once

#include <vector>
#include "Db/DbManager.h"
#include "Models/References/Niche.h"

class CreatorNicheRepository {

private:
    DbManager& dbManager;

public:
    CreatorNicheRepository(DbManager& db);

    // CREATE
    void insert(int creatorId, int nicheId);

    // READ (JOIN)
    std::vector<Niche> getByCreatorId(int creatorId);

    // DELETE (soft)
    void softDelete(int creatorId, int nicheId);
};