//A class which will have all the helper functions for CURD operations 
#pragma once

#include <vector>
#include <string>
#include "../../Models/Brand/Brand.h"
#include "Db/DbManager.h"
#include <optional>
#include <cppconn/connection.h>

class BrandRepository {

private:
    DbManager& dbManager;

public:
    explicit BrandRepository(DbManager& db);

    // ===== CREATE =====
    int insertBrand(
        sql::Connection* conn,
        const std::string& brandName,
        const std::optional<std::string>& psychographics,
        const std::optional<std::string>& genreDescription,
        const std::optional<std::string>& companyType
    );

    // ===== READ =====
    Brand getById(int brandId);
    std::vector<Brand> getAll();

    // ===== UPDATE =====
    void updateName(
        sql::Connection* conn,
        int brandId,
        const std::optional<std::string>& newName
    );

    void updatePsychographics(
        sql::Connection* conn,
        int brandId,
        const std::optional<std::string>& psychographics
    );

    void updateGenreDescription(
        sql::Connection* conn,
        int brandId,
        const std::optional<std::string>& genreDescription
    );

    void updateCompanyType(
        sql::Connection* conn,
        int brandId,
        const std::optional<std::string>& companyType
    );

    // ===== DELETE =====
    void softDelete(
        sql::Connection* conn,
        int brandId
    );

    bool existsByName(
        sql::Connection* conn,
        const std::string& name
    );

    bool existsById(
        sql::Connection* conn,
        int brandId
    );
};