#pragma once

#include <optional>
#include <string>
#include "../../Models/Brand/CompanyType.h"
#include "../DbManager.h"

class CompanyTypeRepository {
private:
    DbManager& dbManager;

public:
    explicit CompanyTypeRepository(DbManager& db);

    // CREATE
    int insert(int brandId, const std::string& type);

    // READ
    std::optional<CompanyType> getByBrandId(int brandId);

    // UPDATE
    void update(int brandId, const std::string& newType);

    // DELETE
    void deleteById(int brandId);
};