#pragma once

#include <vector>
#include <optional>
#include "../../Models/Brand/Revenue.h"
#include "../DbManager.h"
#include <cppconn/connection.h>

class RevenueRepository {
private:
    DbManager& dbManager;

public:
    explicit RevenueRepository(DbManager& db);

    int insert(sql::Connection* conn,
               int brandId,
               int financialYear,
               const std::optional<std::string>& period,
               const std::optional<std::string>& source,
               const std::optional<std::string>& revenueType,
               double value,
               const std::optional<std::string>& currency);

    std::vector<Revenue> getByBrandId(int brandId);
    std::optional<Revenue> getById(int id);

    void update(sql::Connection* conn,
                int revenueId,
                const std::optional<int>& financialYear,
                const std::optional<std::string>& period,
                const std::optional<std::string>& source,
                const std::optional<std::string>& revenueType,
                const std::optional<double>& value,
                const std::optional<std::string>& currency);

    void softDelete(sql::Connection* conn,
                    int revenueId);
};