#pragma once

#include <optional>
#include "../../Models/Brand/Insights.h"
#include "../DbManager.h"
#include <cppconn/connection.h>

class InsightsRepository {
private:
    DbManager& dbManager;

public:
    explicit InsightsRepository(DbManager& db);

    int insert(sql::Connection* conn,
               int brandId,
               const std::optional<std::string>& pitchAngle,
               const std::optional<std::string>& leverage,
               const std::optional<std::string>& gap,
               const std::optional<std::string>& hook);

    std::optional<Insights> getByBrandId(int brandId);

    void update(
        sql::Connection* conn,
        int insightsId,
        const std::optional<std::string>& pitchAngle,
        const std::optional<std::string>& leverage,
        const std::optional<std::string>& gap,
        const std::optional<std::string>& hook
    );

    void softDelete(sql::Connection* conn,
                    int insightsId);
};