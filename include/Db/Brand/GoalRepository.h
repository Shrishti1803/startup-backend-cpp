#pragma once

#include <vector>
#include <optional>
#include "../../Models/Brand/Goal.h"
#include "../DbManager.h"
#include <cppconn/connection.h>

class GoalRepository {
private:
    DbManager& dbManager;

public:
    explicit GoalRepository(DbManager& db);

    int insert(sql::Connection* conn,
               int brandId,
               const std::optional<std::string>& description);

    std::vector<Goal> getByBrandId(int brandId);
    std::optional<Goal> getById(int goalId);

    void update(sql::Connection* conn,
                int goalId,
                const std::optional<std::string>& description);

    void softDelete(sql::Connection* conn,
                    int goalId);
};      