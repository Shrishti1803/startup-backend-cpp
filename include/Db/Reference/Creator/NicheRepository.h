#pragma once

#include <vector>
#include <optional>
#include "Db/DbManager.h"
#include "Models/References/Niche.h"

class NicheRepository {

private:
    DbManager& dbManager;

public:
    NicheRepository(DbManager& db);

    // CREATE
    int insert(const std::string& nicheType);

    // READ
    Niche getById(int nicheId);
    std::vector<Niche> getAll();

    // UPDATE
    void update(int nicheId,
                const std::optional<std::string>& nicheType);

    // DELETE
    void softDelete(int nicheId);
};