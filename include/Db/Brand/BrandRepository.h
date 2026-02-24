//A class which will have all the helper functions for CURD operations 
#pragma once

#include <vector>
#include <string>
#include "../../Models/Brand/Brand.h"
#include "DbManager.h"

class BrandRepository {

private:
    DbManager& dbManager;

public:
    explicit BrandRepository(DbManager& db);

    // ===== CREATE =====
    int insertBrand(const std::string& brandName);

    // ===== READ =====
    Brand getById(int brandId);
    std::vector<Brand> getAll();

    // ===== UPDATE =====
    void updateName(int brandId, const std::string& newName);

    // ===== DELETE =====
    void deleteById(int brandId);
};