#include "Db/Brand/InsightsRepository.h"
#include "Utils/Logger.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>

#include <memory>
#include <vector>
#include <string>

InsightsRepository::InsightsRepository(DbManager& db)
    : dbManager(db) {}

int InsightsRepository::insert(
    sql::Connection* conn,
    int brandId,
    const std::optional<std::string>& pitchAngle,
    const std::optional<std::string>& leverage,
    const std::optional<std::string>& gap,
    const std::optional<std::string>& hook)
{
    auto db_logger = Logger::db();
    db_logger->info("Inserting insights for brand_id={}", brandId);

    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "INSERT INTO INSIGHTS "
                "(brand_id, pitch_angle, leverage, gap, hook) "
                "VALUES (?, ?, ?, ?, ?)"
            )
        );

        pstmt->setInt(1, brandId);

        if (pitchAngle) pstmt->setString(2, pitchAngle.value());
        else pstmt->setNull(2, sql::DataType::VARCHAR);

        if (leverage) pstmt->setString(3, leverage.value());
        else pstmt->setNull(3, sql::DataType::VARCHAR);

        if (gap) pstmt->setString(4, gap.value());
        else pstmt->setNull(4, sql::DataType::VARCHAR);

        if (hook) pstmt->setString(5, hook.value());
        else pstmt->setNull(5, sql::DataType::VARCHAR);

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
        db_logger->error("Insights insert failed: {}", e.what());
        throw;
    }
}

std::optional<Insights>
InsightsRepository::getByBrandId(int brandId)
{
    auto db_logger = Logger::db();
    db_logger->info("Fetching insights for brand_id={}", brandId);

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "SELECT insights_id, pitch_angle, leverage, gap, hook "
                "FROM INSIGHTS "
                "WHERE brand_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setInt(1, brandId);

        std::unique_ptr<sql::ResultSet> rs(pstmt->executeQuery());

        if (rs->next()) {

            std::optional<std::string> pitch, leverage, gap, hook;

            if (!rs->isNull("pitch_angle"))
                pitch = rs->getString("pitch_angle");

            if (!rs->isNull("leverage"))
                leverage = rs->getString("leverage");

            if (!rs->isNull("gap"))
                gap = rs->getString("gap");

            if (!rs->isNull("hook"))
                hook = rs->getString("hook");

            Insights insights(
                rs->getInt("insights_id"),
                pitch,
                leverage,
                gap,
                hook
            );

            return insights;
        }

        return std::nullopt;
    }
    catch (sql::SQLException& e) {
        db_logger->error("Insights fetch failed: {}", e.what());
        throw;
    }
}

void InsightsRepository::update(
    sql::Connection* conn,
    int insightsId,
    const std::optional<std::string>& pitchAngle,
    const std::optional<std::string>& leverage,
    const std::optional<std::string>& gap,
    const std::optional<std::string>& hook)
{
    auto db_logger = Logger::db();
    db_logger->info("Updating insights_id={}", insightsId);

    try {

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "UPDATE INSIGHTS SET "
                "pitch_angle = ?, "
                "leverage = ?, "
                "gap = ?, "
                "hook = ? "
                "WHERE insights_id = ? AND is_deleted = 0"
            )
        );

        // pitch_angle
        if (pitchAngle.has_value())
            pstmt->setString(1, pitchAngle.value());
        else
            pstmt->setNull(1, sql::DataType::VARCHAR);

        // leverage
        if (leverage.has_value())
            pstmt->setString(2, leverage.value());
        else
            pstmt->setNull(2, sql::DataType::VARCHAR);

        // gap
        if (gap.has_value())
            pstmt->setString(3, gap.value());
        else
            pstmt->setNull(3, sql::DataType::VARCHAR);

        // hook
        if (hook.has_value())
            pstmt->setString(4, hook.value());
        else
            pstmt->setNull(4, sql::DataType::VARCHAR);

        pstmt->setInt(5, insightsId);

        int rows = pstmt->executeUpdate();

        if (rows == 0)
            throw std::runtime_error("Insights not found for update");

        db_logger->info("Insights {} updated successfully", insightsId);
    }
    catch (sql::SQLException& e) {
        db_logger->error("Insights update failed: {}", e.what());
        throw;
    }
}

void InsightsRepository::softDelete(
    sql::Connection* conn,
    int insightsId)
{
    auto db_logger = Logger::db();
    db_logger->info("Soft deleting insights_id={}", insightsId);

    try {

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "UPDATE INSIGHTS SET is_deleted = 1 "
                "WHERE insights_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setInt(1, insightsId);

        int rows = pstmt->executeUpdate();

        if (rows == 0)
            throw std::runtime_error("Insights not found for deletion");

        db_logger->info("Insights {} soft deleted", insightsId);
    }
    catch (sql::SQLException& e) {
        db_logger->error("Insights delete failed: {}", e.what());
        throw;
    }
}