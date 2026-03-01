#pragma once

#include <vector>
#include <string>
#include "../../Models/Brand/Competitor.h"
#include "../DbManager.h"

class CompetitorRepository {
private:
    DbManager& dbManager;

public:
    explicit CompetitorRepository(DbManager& db);

    //CREATE
    int insert(int brandId, const std::string& name,
               const std::optional<std::string>& type);

    //READ
    std::vector<Competitor> getByBrandId(int brandId);

    //UPDATE
    void update(int competitorId,
                const std::optional<std::string>& name,
                const std::optional<std::string>& type);

    //DELETE
    void softDelete(int competitorId);

};