#include "Db/Brand/RevenueRepository.h"
#include "Utils/Logger.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>

#include <memory>
#include <vector>
#include <string>

RevenueRepository::RevenueRepository(DbManager& db)
    : dbManager(db) {}

int RevenueRepository::insert(
        int brandId,
        int financialYear,
        const std::optional<std::string>& period,
        const std::optional<std::string>& source,
        const std::optional<std::string>& revenueType,
        double value,
        const std::optional<std::string>& currency)
{
    auto db_logger = Logger::db();
    db_logger->info("Inserting revenue for brand_id={}", brandId);

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "INSERT INTO REVENUE "
                "(brand_id, FinancialYear, revenue_period, RevenueSource, "
                "revenue_type, revenue_value, currency) "
                "VALUES (?, ?, ?, ?, ?, ?, ?)"
            )
        );

        pstmt->setInt(1, brandId);
        pstmt->setInt(2, financialYear);

        if (period) pstmt->setString(3, period.value());
        else pstmt->setNull(3, sql::DataType::VARCHAR);

        if (source) pstmt->setString(4, source.value());
        else pstmt->setNull(4, sql::DataType::VARCHAR);

        if (revenueType) pstmt->setString(5, revenueType.value());
        else pstmt->setNull(5, sql::DataType::VARCHAR);

        pstmt->setDouble(6, value);

        if (currency) pstmt->setString(7, currency.value());
        else pstmt->setNull(7, sql::DataType::VARCHAR);

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
        db_logger->error("Revenue insert failed: {}", e.what());
        throw;
    }
}

std::vector<Revenue>
RevenueRepository::getByBrandId(int brandId)
{
    auto db_logger = Logger::db();
    db_logger->info("Fetching revenue for brand_id={}", brandId);

    std::vector<Revenue> records;

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "SELECT revenue_id, FinancialYear, revenue_period, "
                "RevenueSource, revenue_type, revenue_value, currency "
                "FROM REVENUE "
                "WHERE brand_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setInt(1, brandId);

        std::unique_ptr<sql::ResultSet> rs(pstmt->executeQuery());

        while (rs->next()) {

            std::optional<int> year;
            std::optional<std::string> period, source, type, currency;

            if (!rs->isNull("FinancialYear"))
                year = rs->getInt("FinancialYear");

            if (!rs->isNull("revenue_period"))
                period = rs->getString("revenue_period");

            if (!rs->isNull("RevenueSource"))
                source = rs->getString("RevenueSource");

            if (!rs->isNull("revenue_type"))
                type = rs->getString("revenue_type");

            if (!rs->isNull("currency"))
                currency = rs->getString("currency");

            Revenue record(
                rs->getInt("revenue_id"),
                rs->getDouble("revenue_value"),
                year,
                period,
                source,
                type,
                currency
            );

            records.push_back(record);
        }
    }
    catch (sql::SQLException& e) {
        db_logger->error("Revenue fetch failed: {}", e.what());
        throw;
    }

    return records;
}

void RevenueRepository::update(
        int revenueId,
        const std::optional<int>& financialYear,
        const std::optional<std::string>& period,
        const std::optional<std::string>& source,
        const std::optional<std::string>& revenueType,
        const std::optional<double>& value,
        const std::optional<std::string>& currency)
{
    auto db_logger = Logger::db();
    db_logger->info("Updating revenue_id={}", revenueId);

    if (!financialYear && !period && !source &&
        !revenueType && !value && !currency)
    {
        db_logger->warn("No fields provided for revenue update id={}", revenueId);
        return;
    }

    try {
        auto conn = dbManager.getConnection();

        std::string query = "UPDATE REVENUE SET ";
        std::vector<std::string> fields;

        if (financialYear) fields.push_back("FinancialYear = ?");
        if (period) fields.push_back("revenue_period = ?");
        if (source) fields.push_back("RevenueSource = ?");
        if (revenueType) fields.push_back("revenue_type = ?");
        if (value) fields.push_back("revenue_value = ?");
        if (currency) fields.push_back("currency = ?");

        for (size_t i = 0; i < fields.size(); ++i) {
            query += fields[i];
            if (i < fields.size() - 1)
                query += ", ";
        }

        query += " WHERE revenue_id = ? AND is_deleted = 0";

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(query)
        );

        int index = 1;

        if (financialYear) pstmt->setInt(index++, financialYear.value());
        if (period) pstmt->setString(index++, period.value());
        if (source) pstmt->setString(index++, source.value());
        if (revenueType) pstmt->setString(index++, revenueType.value());
        if (value) pstmt->setDouble(index++, value.value());
        if (currency) pstmt->setString(index++, currency.value());

        pstmt->setInt(index, revenueId);

        int rows = pstmt->executeUpdate();

        if (rows == 0){
            db_logger->warn("No revenue found for update id={}", revenueId);
            throw std::runtime_error("Revenue not found for update");
        }

        db_logger->info("Revenue {} updated successfully", revenueId);
    }
    catch (sql::SQLException& e) {
        db_logger->error("Revenue update failed: {}", e.what());
        throw;
    }
}

void RevenueRepository::softDelete(int revenueId)
{
    auto db_logger = Logger::db();
    db_logger->info("Soft deleting revenue_id={}", revenueId);

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "UPDATE REVENUE SET is_deleted = 1 "
                "WHERE revenue_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setInt(1, revenueId);

        int rows = pstmt->executeUpdate();

        if (rows == 0)
            throw std::runtime_error("Revenue not found for deletion");

        db_logger->info("Revenue {} soft deleted", revenueId);
    }
    catch (sql::SQLException& e) {
        db_logger->error("Revenue delete failed: {}", e.what());
        throw;
    }
}