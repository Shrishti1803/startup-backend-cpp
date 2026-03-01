#pragma once

#include <vector>
#include "../../Models/Brand/Revenue.h"
#include "../DbManager.h"

class RevenueRepository {
private:
    DbManager& dbManager;

public:
    explicit RevenueRepository(DbManager& db);

    int insert(int brandId,
               int financialYear,
               const std::optional<std::string>& period,
               const std::optional<std::string>& source,
               const std::optional<std::string>& revenueType,
               double value,
               const std::optional<std::string>& currency);

    std::vector<Revenue> getByBrandId(int brandId);

    void update(int revenueId,
            const std::optional<int>& financialYear,
            const std::optional<std::string>& period,
            const std::optional<std::string>& source,
            const std::optional<std::string>& revenueType,
            const std::optional<double>& value,
            const std::optional<std::string>& currency);

    void softDelete(int revenueId);

};