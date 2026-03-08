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

int BrandRepository::insertBrand(const std::string& brandName) {
    auto db_logger = Logger::db();
    db_logger->info("Inserting brand: {}", brandName);

    try {
        sql::Connection* conn = dbManager.getConnection();

        if (!conn) {
            db_logger->error("Database connection is null");
            throw std::runtime_error("Database connection not established");
        }

        // Insert brand
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement("INSERT INTO BRAND (brand_name) VALUES (?)")
        );

        pstmt->setString(1, brandName);
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
                "SELECT brand_id, brand_name FROM BRAND WHERE brand_id = ? AND is_deleted = 0 "
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

            db_logger->info("Brand found: {}", name);
            return Brand(id, name);
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
                "SELECT brand_id, brand_name FROM BRAND WHERE is_deleted = 0 "
            )
        );

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
            int id = res->getInt("brand_id");
            std::string name = res->getString("brand_name");

            brands.emplace_back(id, name);
        }

        db_logger->info("Total brands fetched: {}", brands.size());
        return brands;
    }
    catch (sql::SQLException& e) {
        db_logger->error("SQL Exception while fetching all brands: {}", e.what());
        throw;
    }
}


void BrandRepository::updateName(int brandId, const std::optional<std::string>& newName) {
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
        sql::Connection* conn = dbManager.getConnection();

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

        int rowsAffected = pstmt->executeUpdate(); //Check if the data is updated 

        if (rowsAffected == 0) {
            db_logger->warn("No brand found to update with ID {}", brandId);
            throw std::runtime_error("Brand not found for update");
        }

        db_logger->info("Brand ID {} updated successfully", brandId);
    }
    catch (sql::SQLException& e) {
        db_logger->error("SQL Exception while updating brand: {}", e.what());
        throw;
    }
}


void BrandRepository::softDelete(int brandId) {
    auto db_logger = Logger::db();
    db_logger->info("SOft deleting brand with ID {}", brandId);

    try {
        sql::Connection* conn = dbManager.getConnection();

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