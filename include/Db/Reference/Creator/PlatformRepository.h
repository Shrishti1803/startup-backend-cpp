#pragma once

#include <vector>
#include <optional>
#include "Db/DbManager.h"
#include "Models/References/Platform.h"

class PlatformRepository {

private:
    DbManager& dbManager;

public:
    PlatformRepository(DbManager& db);

    // CREATE
    int insert(const std::string& platformName);

    // READ
    Platform getById(int platformId);
    std::vector<Platform> getAll();

    // UPDATE
    void update(int platformId,
                const std::optional<std::string>& platformName);

    // DELETE
    void softDelete(int platformId);
};