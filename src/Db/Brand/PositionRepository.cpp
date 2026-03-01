#include "Db/Brand/PositionRepository.h"
#include "Utils/Logger.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>

#include <memory>
#include <vector>
#include <string>

PositionRepository::PositionRepository(DbManager& db)
    : dbManager(db) {}

int PositionRepository::insert(const std::string& positionName)
{
    auto db_logger = Logger::db();
    db_logger->info("Inserting position: {}", positionName);

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "INSERT INTO B_POSITION (position_name) VALUES (?)"
            )
        );

        pstmt->setString(1, positionName);
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
        db_logger->error("Position insert failed: {}", e.what());
        throw;
    }
}

std::vector<Position> PositionRepository::getAll()
{
    auto db_logger = Logger::db();
    db_logger->info("Fetching all positions");

    std::vector<Position> positions;

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "SELECT position_id, position_name "
                "FROM B_POSITION "
                "WHERE is_deleted = 0"
            )
        );

        std::unique_ptr<sql::ResultSet> rs(pstmt->executeQuery());

        while (rs->next()) {
            Position pos(
                rs->getInt("position_id"),
                rs->getString("position_name")
            );

            positions.push_back(pos);
        }
    }
    catch (sql::SQLException& e) {
        db_logger->error("Position fetch failed: {}", e.what());
        throw;
    }

    return positions;
}

std::optional<Position>
PositionRepository::getById(int positionId)
{
    auto db_logger = Logger::db();
    db_logger->info("Fetching position_id={}", positionId);

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "SELECT position_id, position_name "
                "FROM B_POSITION "
                "WHERE position_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setInt(1, positionId);

        std::unique_ptr<sql::ResultSet> rs(pstmt->executeQuery());

        if (rs->next()) {
            Position pos(
                rs->getInt("position_id"),
                rs->getString("position_name")
            );
            return pos;
        }

        return std::nullopt;
    }
    catch (sql::SQLException& e) {
        db_logger->error("Position fetch by id failed: {}", e.what());
        throw;
    }
}

void PositionRepository::softDelete(int positionId)
{
    auto db_logger = Logger::db();
    db_logger->info("Soft deleting position_id={}", positionId);

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "UPDATE B_POSITION SET is_deleted = 1 "
                "WHERE position_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setInt(1, positionId);

        int rows = pstmt->executeUpdate();

        if (rows == 0)
            throw std::runtime_error("Position not found for deletion");

        db_logger->info("Position {} soft deleted", positionId);
    }
    catch (sql::SQLException& e) {
        db_logger->error("Position delete failed: {}", e.what());
        throw;
    }
}

