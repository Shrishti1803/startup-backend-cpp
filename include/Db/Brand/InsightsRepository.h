#pragma once

#include <optional>
#include "../../Models/Brand/Insights.h"
#include "../DbManager.h"

class InsightsRepository {
private:
    DbManager& dbManager;

public:
    explicit InsightsRepository(DbManager& db);

    int insert(int brandId,
               const std::optional<std::string>& pitchAngle,
               const std::optional<std::string>& leverage,
               const std::optional<std::string>& gap,
               const std::optional<std::string>& hook);

    std::optional<Insights> getByBrandId(int brandId);

    void update(
    int insightsId,
    const std::optional<std::string>& pitchAngle,
    const std::optional<std::string>& leverage,
    const std::optional<std::string>& gap,
    const std::optional<std::string>& hook);

    void softDelete(int insights_id);

};