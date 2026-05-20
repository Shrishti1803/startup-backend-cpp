/*
This repository along with all the other repositories are reponsible for the 
Db connection and implementing SQL queries of the CURD operations through Prepared statements 
Soft delete and PATCH style update is constant in all the Repositores 

The unique_ptr is for RAII implementation 
makes sure there are no memory leaks 

*/
#include "Db/Brand/BrandRepository.h"
#include "Utils/Logger.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

BrandRepository::BrandRepository(DbManager& db)
    : dbManager(db) {} //Constructor call for Db Access


int BrandRepository::insertBrand(
    sql::Connection* conn,
    const std::string& brandName,
    const std::optional<std::string>& psychographics,
    const std::optional<std::string>& genreDescription,
    const std::optional<std::string>& companyType)
{
    auto db_logger = Logger::db();
    db_logger->info("Inserting brand: {}", brandName);

    try {
        if (!conn) {
            db_logger->error("Database connection is null");
            throw std::runtime_error("Database connection not established");
        }

        // Insert brand
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("INSERT INTO BRAND (brand_name, psychographics, genre_description, company_type) VALUES (?, ?, ?, ?)")
        );

        pstmt->setString(1, brandName);
        if (psychographics.has_value())
            pstmt->setString(2, psychographics.value());
        else
            pstmt->setNull(2, sql::DataType::VARCHAR);

        if (genreDescription.has_value())
            pstmt->setString(3, genreDescription.value());
        else
            pstmt->setNull(3, sql::DataType::VARCHAR);
        if(companyType.has_value())
            pstmt->setString(4, companyType.value());
        else
            pstmt->setNull(4, sql::DataType::VARCHAR);
        pstmt->executeUpdate();

        // Get generated ID so that it can be verified that data is inserted in the DB table
        std::unique_ptr<sql::PreparedStatement> idStmt(
            conn->prepareStatement("SELECT LAST_INSERT_ID()")
        );

        std::unique_ptr<sql::ResultSet> res(idStmt->executeQuery());

        int generatedId = 0;
        if (res->next()) {
            generatedId = res->getInt(1);
        }

        db_logger->info("Brand inserted successfully with ID {}", generatedId);
        return generatedId;
    }
    catch (sql::SQLException& e) {
        db_logger->error("SQL Exception while inserting brand: {}", e.what());
        throw;
    }
}


Brand BrandRepository::getById(int brandId) {
    auto db_logger = Logger::db();
    db_logger->info("Fetching brand with ID {}", brandId);

    try {
        sql::Connection* conn = dbManager.getConnection();

        if (!conn) {
            db_logger->error("Database connection is null");
            throw std::runtime_error("Database connection not established");
        }

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "SELECT brand_id, brand_name, psychographics, genre_description, company_type FROM BRAND WHERE brand_id = ? AND is_deleted = 0 "
                //here is_deleted = 0 is used to check the records which have not been deleted 
                //is_deleted is a column in every table which will be set to 1 when a user does Partial Delete
                //so while accessing the data, is_deleted = 0 are the records that are not soft deleted
            )
        );

        pstmt->setInt(1, brandId);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            int id = res->getInt("brand_id");
            std::string name = res->getString("brand_name");

            std::optional<std::string> psych;
            if (!res->isNull("psychographics")) {
                psych = std::string(res->getString("psychographics"));
            }

            std::optional<std::string> genreDesc;

            if (!res->isNull("genre_description")) {
                genreDesc = res->getString("genre_description");
            }
            std::optional<std::string> companyType;

            if(!res->isNull("company_type"))
            companyType = res->getString("company_type");

            return Brand(id,name,psych,genreDesc,companyType);

            db_logger->info("Brand found: {}", name);
            return Brand(id, name, psych, genreDesc);
        }
        

        db_logger->warn("Brand with ID {} not found", brandId);
        throw std::runtime_error("Brand not found");
    }
    catch (sql::SQLException& e) {
        db_logger->error("SQL Exception while fetching brand: {}", e.what());
        throw;
    }
}

std::vector<Brand> BrandRepository::getAll() {
    auto db_logger = Logger::db();
    db_logger->info("Fetching all brands");

    std::vector<Brand> brands;

    try {
        sql::Connection* conn = dbManager.getConnection();

        if (!conn) {
            db_logger->error("Database connection is null");
            throw std::runtime_error("Database connection not established");
        }

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "SELECT brand_id, brand_name, psychographics FROM BRAND WHERE is_deleted = 0 "
            )
        );

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        while (res->next()) {
            int id = res->getInt("brand_id");
            std::string name = res->getString("brand_name");

            std::optional<std::string> psych;

            if (!res->isNull("psychographics")) {
                psych = std::string(res->getString("psychographics"));
            }

            brands.emplace_back(id, name, psych);
        }

        db_logger->info("Total brands fetched: {}", brands.size());
        return brands;
    }
    catch (sql::SQLException& e) {
        db_logger->error("SQL Exception while fetching all brands: {}", e.what());
        throw;
    }
}


void BrandRepository::updateName(
    sql::Connection* conn,
    int brandId,
    const std::optional<std::string>& newName)
{
    auto db_logger = Logger::db();
    //First it is checked if the column has any value. If it doesn't have any value then we simply don't update anything. 
    if (!newName.has_value()) {
        db_logger->warn("No update performed for brand ID {}: newName is empty", brandId);
        return;
    }

    db_logger->info(
        "Updating brand ID {} with new name {}",
        brandId,
        newName.has_value() ? newName.value() : "NULL"
    );

    try {

        if (!conn) {
            db_logger->error("Database connection is null");
            throw std::runtime_error("Database connection not established");
        }

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "UPDATE BRAND SET brand_name = ? WHERE brand_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setString(1, newName.value());
        pstmt->setInt(2, brandId);

        int rowsAffected = pstmt->executeUpdate();

        // 0 can mean same value already exists.
        // So verify existence separately.

        if (rowsAffected == 0 && !existsById(conn, brandId)) {
            throw std::runtime_error("Brand not found for update");
        }

        db_logger->info("Brand ID {} updated successfully", brandId);
    }
    catch (sql::SQLException& e) {
        db_logger->error("SQL Exception while updating brand: {}", e.what());
        throw;
    }
}

void BrandRepository::updatePsychographics(
    sql::Connection* conn,
    int brandId,
    const std::optional<std::string>& psychographics)
{
    auto db_logger = Logger::db();

    if (!psychographics.has_value()) {
        db_logger->warn("No update for psychographics for brand {}", brandId);
        return;
    }

    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "UPDATE BRAND SET psychographics = ? WHERE brand_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setString(1, psychographics.value());
        pstmt->setInt(2, brandId);

        int rows = pstmt->executeUpdate();

        if (rows == 0) {
            throw std::runtime_error("Brand not found for psychographics update");
        }
    }
    catch (sql::SQLException& e) {
        Logger::db()->error("Psychographics update failed: {}", e.what());
        throw;
    }
}

void BrandRepository::updateGenreDescription(
    sql::Connection* conn,
    int brandId,
    const std::optional<std::string>& genreDescription)
{
    if (!genreDescription.has_value()) return;

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "UPDATE BRAND SET genre_description = ? WHERE brand_id = ? AND is_deleted = 0"
        )
    );

    pstmt->setString(1, genreDescription.value());
    pstmt->setInt(2, brandId);

    pstmt->executeUpdate();
}


void BrandRepository::softDelete(
    sql::Connection* conn,
    int brandId)
{
    auto db_logger = Logger::db();
    db_logger->info("SOft deleting brand with ID {}", brandId);

    try {
        if (!conn) {
            db_logger->error("Database connection is null");
            throw std::runtime_error("Database connection not established");
        }

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "UPDATE BRAND SET is_deleted = 1 WHERE brand_id = ? AND is_deleted = 0" // we simply set is_deleted = 1 when we wanna delete a particular record 
            )
        );

        pstmt->setInt(1, brandId);

        int rowsAffected = pstmt->executeUpdate();

        if (rowsAffected == 0) {
            db_logger->warn("No brand found to delete with ID {}", brandId);
            throw std::runtime_error("Brand not found for deletion");
        }

        db_logger->info("Brand ID {} deleted successfully", brandId);
    }
    catch (sql::SQLException& e) {
        db_logger->error("SQL Exception while deleting brand: {}", e.what());
        throw;
    }
}

bool BrandRepository::existsByName(sql::Connection* conn, const std::string& name)
{
    std::unique_ptr<sql::PreparedStatement> stmt(
        conn->prepareStatement(
            "SELECT COUNT(*) FROM BRAND WHERE brand_name = ? AND is_deleted = 0"
        )
    );

    stmt->setString(1, name);

    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

    if (res->next()) {
        return res->getInt(1) > 0;
    }

    return false;
}

bool BrandRepository::existsById(sql::Connection* conn, int brandId)
{
    std::unique_ptr<sql::PreparedStatement> stmt(
        conn->prepareStatement(
            "SELECT 1 FROM BRAND WHERE brand_id = ? AND is_deleted = 0 LIMIT 1"
        )
    );

    stmt->setInt(1, brandId);

    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

    return res->next();
}

void BrandRepository::updateCompanyType(
    [[maybe_unused]] sql::Connection* conn,
    int brandId,
    const std::optional<std::string>& companyType)
{
    auto db_logger = Logger::db();

    if (!companyType.has_value()) {
        db_logger->warn("No update for company type for brand {}", brandId);
        return;
    }

    try {
        sql::Connection* conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "UPDATE BRAND SET company_type = ? WHERE brand_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setString(1, companyType.value());
        pstmt->setInt(2, brandId);

        int rows = pstmt->executeUpdate();

        if (rows == 0) {
            throw std::runtime_error("Brand not found for company type update");
        }

    } catch (sql::SQLException& e) {
        db_logger->error("Company type update failed: {}", e.what());
        throw;
    }
}