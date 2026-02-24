#include "Db/Brand/BrandRepository.h"
#include "Utils/Logger.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

BrandRepository::BrandRepository(DbManager& db)
    : dbManager(db) {}

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

        // Get generated ID
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
                "SELECT brand_id, brand_name FROM BRAND WHERE brand_id = ?"
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
                "SELECT brand_id, brand_name FROM BRAND"
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


void BrandRepository::updateName(int brandId, const std::string& newName) {
    auto db_logger = Logger::db();
    db_logger->info("Updating brand ID {} with new name {}", brandId, newName);

    try {
        sql::Connection* conn = dbManager.getConnection();

        if (!conn) {
            db_logger->error("Database connection is null");
            throw std::runtime_error("Database connection not established");
        }

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "UPDATE BRAND SET brand_name = ? WHERE brand_id = ?"
            )
        );

        pstmt->setString(1, newName);
        pstmt->setInt(2, brandId);

        int rowsAffected = pstmt->executeUpdate();

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


void BrandRepository::deleteById(int brandId) {
    auto db_logger = Logger::db();
    db_logger->info("Deleting brand with ID {}", brandId);

    try {
        sql::Connection* conn = dbManager.getConnection();

        if (!conn) {
            db_logger->error("Database connection is null");
            throw std::runtime_error("Database connection not established");
        }

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "DELETE FROM BRAND WHERE brand_id = ?"
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