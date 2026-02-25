#pragma once

#include <vector>
#include <optional>
#include "../../Models/Brand/Goal.h"
#include "../DbManager.h"

class GoalRepository {
private:
    DbManager& dbManager;

public:
    explicit GoalRepository(DbManager& db);

    int insert(int brandId,
               const std::optional<std::string>& description);

    std::vector<Goal> getByBrandId(int brandId);

    void update(int goalId,
                const std::optional<std::string>& description);

    void deleteById(int goalId);

};