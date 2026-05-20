#pragma once

#include <vector>
#include <optional>
#include "Db/DbManager.h"
#include "Models/References/TargetAudience.h"

class TargetAudienceRepository {

private:
    DbManager& dbManager;

public:
    TargetAudienceRepository(DbManager& db);

    // CREATE
    int insert(const std::string& name);

    // READ
    TargetAudience getById(int audId);
    std::vector<TargetAudience> getAll();

    // UPDATE
    void update(int audId,
                const std::optional<std::string>& name);

    // DELETE
    void softDelete(int audId);

};