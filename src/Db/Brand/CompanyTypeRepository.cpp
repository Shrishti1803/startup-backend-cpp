#include "Db/Brand/CompanyTypeRepository.h"
#include "Utils/Logger.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

CompanyTypeRepository::CompanyTypeRepository(DbManager& db)
    : dbManager(db) {}

int CompanyTypeRepository::insert(int brandId,
                                   const std::string& type)
{
    auto db_logger = Logger::db();
    db_logger->info("Inserting company type for brand_id={}", brandId);

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "INSERT INTO COMPANY_TYPE (brand_id, type) VALUES (?, ?)"
            )
        );

        pstmt->setInt(1, brandId);
        pstmt->setString(2, type);

        pstmt->executeUpdate();

        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> rs(
            stmt->executeQuery("SELECT LAST_INSERT_ID()")
        );

        if (rs->next())
            return rs->getInt(1);

        return -1;
    }
    catch (sql::SQLException& e) {
        db_logger->error("CompanyType insert failed: {}", e.what());
        throw;
    }
}

std::optional<CompanyType>
CompanyTypeRepository::getByBrandId(int brandId)
{
    auto db_logger = Logger::db();
    db_logger->info("Fetching company type for brand_id={}", brandId);

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "SELECT comp_type_id, brand_id, type "
                "FROM COMPANY_TYPE WHERE brand_id = ?"
            )
        );

        pstmt->setInt(1, brandId);

        std::unique_ptr<sql::ResultSet> rs(
            pstmt->executeQuery()
        );

        if (rs->next()) {
            CompanyType ct(
                rs->getInt("comp_type_id"),
                std::string(rs->getString("type"))
            );

            return ct;
        }

        return std::nullopt;
    }
    catch (sql::SQLException& e) {
        db_logger->error("CompanyType fetch failed: {}", e.what());
        throw;
    }
}

std::optional<CompanyType>
CompanyTypeRepository::getByBrandId(int brandId)
{
    auto db_logger = Logger::db();
    db_logger->info("Fetching company type for brand_id={}", brandId);

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "SELECT comp_type_id, brand_id, type "
                "FROM COMPANY_TYPE WHERE brand_id = ?"
            )
        );

        pstmt->setInt(1, brandId);

        std::unique_ptr<sql::ResultSet> rs(
            pstmt->executeQuery()
        );

        if (rs->next()) {
            CompanyType ct(
                rs->getInt("comp_type_id"),
                std::string(rs->getString("type"))
            );

            return ct;
        }

        return std::nullopt;
    }
    catch (sql::SQLException& e) {
        db_logger->error("CompanyType fetch failed: {}", e.what());
        throw;
    }
}

void CompanyTypeRepository::deleteById(int id)
{
    auto db_logger = Logger::db();
    db_logger->info("Deleting company type id={}", id);

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "DELETE FROM COMPANY_TYPE WHERE comp_type_id = ?"
            )
        );

        pstmt->setInt(1, id);
        pstmt->executeUpdate();
    }
    catch (sql::SQLException& e) {
        db_logger->error("CompanyType delete failed: {}", e.what());
        throw;
    }
}