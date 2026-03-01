#include "Db/Brand/CompetitorRepository.h"
#include "Utils/Logger.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

CompetitorRepository::CompetitorRepository(DbManager& db)
    : dbManager(db) {}

int CompetitorRepository::insert(
        int brandId,
        const std::string& name,
        const std::optional<std::string>& type)
{
    auto db_logger = Logger::db();
    db_logger->info("Inserting competitor for brand_id={}", brandId);

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "INSERT INTO COMPETITORS (brand_id, name, type) "
                "VALUES (?, ?, ?)"
            )
        );

        pstmt->setInt(1, brandId);
        pstmt->setString(2, name);

        if (type.has_value())
            pstmt->setString(3, type.value());
        else
            pstmt->setNull(3, sql::DataType::VARCHAR);

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
        db_logger->error("Competitor insert failed: {}", e.what());
        throw;
    }
}

std::vector<Competitor>
CompetitorRepository::getByBrandId(int brandId)
{
    auto db_logger = Logger::db();
    db_logger->info("Fetching competitors for brand_id={}", brandId);

    std::vector<Competitor> competitors;

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "SELECT competitors_id, brand_id, name, type "
                "FROM COMPETITORS WHERE brand_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setInt(1, brandId);

        std::unique_ptr<sql::ResultSet> rs(
            pstmt->executeQuery()
        );

        while (rs->next()) {

            std::optional<std::string> type;

            if (!rs->isNull("type")) {
                type = std::string(rs->getString("type"));
            }

            Competitor competitor(
                rs->getInt("competitors_id"),
                std::string(rs->getString("name")),
                type
            );

            competitors.push_back(competitor);
        }
    }
    catch (sql::SQLException& e) {
        db_logger->error("Competitor fetch failed: {}", e.what());
        throw;
    }

    return competitors;
}

void CompetitorRepository::update(
        int competitorId,
        const std::optional<std::string>& name,
        const std::optional<std::string>& type)
{
    auto db_logger = Logger::db();
    db_logger->info("Updating competitor_id={}", competitorId);

    if (!name.has_value() && !type.has_value()) {
        db_logger->warn("No fields provided to update for competitor_id={}", competitorId);
        return;
    }

    try {
        auto conn = dbManager.getConnection();

        std::string query = "UPDATE COMPETITORS SET ";
        std::vector<std::string> fields;

        if (name.has_value())
            fields.push_back("name = ?");

        if (type.has_value())
            fields.push_back("type = ?");

        for (size_t i = 0; i < fields.size(); ++i) {
            query += fields[i];
            if (i < fields.size() - 1)
                query += ", ";
        }

        query += " WHERE competitors_id = ? AND is_deleted = 0";

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(query)
        );

        int paramIndex = 1;

        if (name.has_value())
            pstmt->setString(paramIndex++, name.value());

        if (type.has_value())
            pstmt->setString(paramIndex++, type.value());

        pstmt->setInt(paramIndex, competitorId);

        int rowsAffected = pstmt->executeUpdate();

        if (rowsAffected == 0) {
            db_logger->warn("No competitor found to update with ID {}", competitorId);
            throw std::runtime_error("Competitor not found for update");
        }

        db_logger->info("Competitor {} updated successfully", competitorId);
    }
    catch (sql::SQLException& e) {
        db_logger->error("Competitor update failed: {}", e.what());
        throw;
    }
}

void CompetitorRepository::softDelete(int competitorId)
{
    auto db_logger = Logger::db();
    db_logger->info("Soft deleting competitor_id={}", competitorId);

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "UPDATE COMPETITORS "
                "SET is_deleted = 1 "
                "WHERE competitors_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setInt(1, competitorId);

        int rowsAffected = pstmt->executeUpdate();

        if (rowsAffected == 0) {
            db_logger->warn("No competitor found to delete with ID {}", competitorId);
            throw std::runtime_error("Competitor not found for deletion");
        }

        db_logger->info("Competitor {} soft deleted successfully", competitorId);
    }
    catch (sql::SQLException& e) {
        db_logger->error("Competitor delete failed: {}", e.what());
        throw;
    }
}