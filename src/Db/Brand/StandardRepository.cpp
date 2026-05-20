#include "Db/Brand/StandardRepository.h"
#include "Utils/Logger.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>

#include <memory>
#include <vector>
#include <string>

BrandStandardRepository::BrandStandardRepository(DbManager& db)
    : dbManager(db) {}

int BrandStandardRepository::insert(
    sql::Connection* conn,
    int brandId,
    const std::optional<std::string>& position,
    const std::optional<std::string>& identity,
    const std::optional<std::string>& csr,
    const std::optional<std::string>& quality,
    const std::optional<std::string>& perception)
{
    auto db_logger = Logger::db();
    db_logger->info("Inserting standard for brand_id={}", brandId);

    try {

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "INSERT INTO BRAND_STANDARD "
                "(brand_id, position, brand_identity, csr, quality, perception) "
                "VALUES (?, ?, ?, ?, ?, ?)"
            )
        );

        pstmt->setInt(1, brandId);

        if (position) pstmt->setString(2, position.value());
        else pstmt->setNull(2, sql::DataType::VARCHAR);

        if (identity) pstmt->setString(3, identity.value());
        else pstmt->setNull(3, sql::DataType::VARCHAR);

        if (csr) pstmt->setString(4, csr.value());
        else pstmt->setNull(4, sql::DataType::VARCHAR);

        if (quality) pstmt->setString(5, quality.value());
        else pstmt->setNull(5, sql::DataType::VARCHAR);

        if (perception) pstmt->setString(6, perception.value());
        else pstmt->setNull(6, sql::DataType::VARCHAR);

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
        db_logger->error("Standard insert failed: {}", e.what());
        throw;
    }
}

std::optional<Standard>
BrandStandardRepository::getByBrandId(int brandId)
{
    auto db_logger = Logger::db();
    db_logger->info("Fetching standard for brand_id={}", brandId);

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "SELECT standard_id, position, brand_identity, csr, quality, perception "
                "FROM BRAND_STANDARD "
                "WHERE brand_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setInt(1, brandId);

        std::unique_ptr<sql::ResultSet> rs(pstmt->executeQuery());

        if (rs->next()) {

            std::optional<std::string> pos, identity, csrVal, qualityVal, perceptionVal;

            if (!rs->isNull("position"))
                pos = rs->getString("position");

            if (!rs->isNull("brand_identity"))
                identity = rs->getString("brand_identity");

            if (!rs->isNull("csr"))
                csrVal = rs->getString("csr");

            if (!rs->isNull("quality"))
                qualityVal = rs->getString("quality");

            if (!rs->isNull("perception"))
                perceptionVal = rs->getString("perception");

            Standard stdObj(
                rs->getInt("standard_id"),
                pos,
                identity,
                csrVal,
                qualityVal,
                perceptionVal
            );

            return stdObj;
        }

        return std::nullopt;
    }
    catch (sql::SQLException& e) {
        db_logger->error("Standard fetch failed: {}", e.what());
        throw;
    }
}

void BrandStandardRepository::update(
    sql::Connection* conn,
    int standardId,
    const std::optional<std::string>& position,
    const std::optional<std::string>& identity,
    const std::optional<std::string>& csr,
    const std::optional<std::string>& quality,
    const std::optional<std::string>& perception)
{
    auto db_logger = Logger::db();
    db_logger->info("Updating standard_id={}", standardId);

    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "UPDATE BRAND_STANDARD SET "
                "position = ?, "
                "brand_identity = ?, "
                "csr = ?, "
                "quality = ?, "
                "perception = ? "
                "WHERE standard_id = ? AND is_deleted = 0"
            )
        );

        // position
        if (position.has_value())
            pstmt->setString(1, position.value());
        else
            pstmt->setNull(1, sql::DataType::VARCHAR);

        // identity
        if (identity.has_value())
            pstmt->setString(2, identity.value());
        else
            pstmt->setNull(2, sql::DataType::VARCHAR);

        // csr
        if (csr.has_value())
            pstmt->setString(3, csr.value());
        else
            pstmt->setNull(3, sql::DataType::VARCHAR);

        // quality
        if (quality.has_value())
            pstmt->setString(4, quality.value());
        else
            pstmt->setNull(4, sql::DataType::VARCHAR);

        // perception
        if (perception.has_value())
            pstmt->setString(5, perception.value());
        else
            pstmt->setNull(5, sql::DataType::VARCHAR);

        pstmt->setInt(6, standardId);

        int rows = pstmt->executeUpdate();

        if (rows == 0)
            throw std::runtime_error("Standard not found for update");

        db_logger->info("Standard {} updated successfully", standardId);
    }
    catch (sql::SQLException& e) {
        db_logger->error("Standard update failed: {}", e.what());
        throw;
    }
}

void BrandStandardRepository::softDelete(
    sql::Connection* conn,
    int standardId)
{
    auto db_logger = Logger::db();
    db_logger->info("Soft deleting standard_id={}", standardId);

    try {

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "UPDATE BRAND_STANDARD SET is_deleted = 1 "
                "WHERE standard_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setInt(1, standardId);

        int rows = pstmt->executeUpdate();

        if (rows == 0)
            throw std::runtime_error("Standard not found for deletion");

        db_logger->info("Standard {} soft deleted", standardId);
    }
    catch (sql::SQLException& e) {
        db_logger->error("Standard delete failed: {}", e.what());
        throw;
    }
}