#pragma once

#include <vector>
#include <string>
#include <optional>
#include "../../Models/Brand/Position.h"
#include "../DbManager.h"

class PositionRepository {
private:
    DbManager& dbManager;

public:
    explicit PositionRepository(DbManager& db);

    int insert(const std::string& positionName);

    std::vector<Position> getAll();

    std::optional<Position> getById(int positionId);

    void softDelete(int positionId);
};