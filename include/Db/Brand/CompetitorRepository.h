#pragma once

#include <vector>
#include <string>
#include <optional>
#include "../../Models/Brand/Competitor.h"
#include "../DbManager.h"
#include <cppconn/connection.h>

class CompetitorRepository {
private:
    DbManager& dbManager;

public:
    explicit CompetitorRepository(DbManager& db);

    // CREATE
    int insert(sql::Connection* conn,
               int brandId,
               const std::string& name,
               const std::optional<std::string>& type);

    // READ
    std::vector<Competitor> getByBrandId(int brandId);
    std::optional<Competitor> getById(int competitorId);

    // UPDATE
    void update(sql::Connection* conn,
                int competitorId,
                const std::optional<std::string>& name,
                const std::optional<std::string>& type);

    // DELETE
    void softDelete(sql::Connection* conn,
                    int competitorId);
};