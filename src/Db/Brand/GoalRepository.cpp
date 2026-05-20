#include "Db/Brand/GoalRepository.h"
#include "Utils/Logger.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

GoalRepository::GoalRepository(DbManager& db)
    : dbManager(db) {}

int GoalRepository::insert(
    sql::Connection* conn,
    int brandId,
    const std::optional<std::string>& description)
{
    auto db_logger = Logger::db();
    db_logger->info("Inserting goal for brand_id={}", brandId);

    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "INSERT INTO GOALS (brand_id, description) VALUES (?, ?)"
            )
        );

        pstmt->setInt(1, brandId);

        if (description.has_value())
            pstmt->setString(2, description.value());
        else
            pstmt->setNull(2, sql::DataType::VARCHAR);

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
        db_logger->error("Goal insert failed: {}", e.what());
        throw;
    }
}

std::vector<Goal> GoalRepository::getByBrandId(int brandId)
{
    auto db_logger = Logger::db();
    db_logger->info("Fetching goals for brand_id={}", brandId);

    std::vector<Goal> goals;

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "SELECT goal_id, brand_id, description "
                "FROM GOALS WHERE brand_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setInt(1, brandId);

        std::unique_ptr<sql::ResultSet> rs(
            pstmt->executeQuery()
        );

        while (rs->next()) {

            std::optional<std::string> desc;

            if (!rs->isNull("description")) {
                desc = std::string(rs->getString("description"));
            }

            Goal goal(
                rs->getInt("goal_id"),
                desc
            );

            goals.push_back(goal);
        }
    }
    catch (sql::SQLException& e) {
        db_logger->error("Goal fetch failed: {}", e.what());
        throw;
    }

    return goals;
}


void GoalRepository::update(
    sql::Connection* conn,
    int goalId,
    const std::optional<std::string>& description)
{
    auto db_logger = Logger::db();

    if (!description.has_value()) {
        db_logger->warn("No update performed for goal_id {}: description is empty", goalId);
        return;
    }

    db_logger->info("Updating goal_id={}", goalId);

    try {

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "UPDATE GOALS "
                "SET description = ? "
                "WHERE goal_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setString(1, description.value());
        pstmt->setInt(2, goalId);

        int rowsAffected = pstmt->executeUpdate();

        if (rowsAffected == 0) {
            db_logger->warn("No goal found to update with ID {}", goalId);
            throw std::runtime_error("Goal not found for update");
        }

        db_logger->info("Goal ID {} updated successfully", goalId);
    }
    catch (sql::SQLException& e) {
        db_logger->error("Goal update failed: {}", e.what());
        throw;
    }
}

void GoalRepository::softDelete(
    sql::Connection* conn,
    int goalId)
{
    auto db_logger = Logger::db();
    db_logger->info("Soft deleting goal_id={}", goalId);

    try {

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "UPDATE GOALS "
                "SET is_deleted = 1 "
                "WHERE goal_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setInt(1, goalId);

        int rowsAffected = pstmt->executeUpdate();

        if (rowsAffected == 0) {
            db_logger->warn("No goal found to delete with ID {}", goalId);
            throw std::runtime_error("Goal not found for deletion");
        }

        db_logger->info("Goal ID {} soft deleted successfully", goalId);
    }
    catch (sql::SQLException& e) {
        db_logger->error("Goal delete failed: {}", e.what());
        throw;
    }
}
std::optional<Goal>
GoalRepository::getById(int goalId)
{
    auto db_logger = Logger::db();

    db_logger->info(
        "Fetching goal by id={}",
        goalId
    );

    try
    {
        auto conn =
            dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                R"(
                    SELECT
                        goal_id,
                        description
                    FROM GOALS
                    WHERE goal_id = ?
                    AND is_deleted = 0
                )"
            )
        );

        pstmt->setInt(1, goalId);

        std::unique_ptr<sql::ResultSet> res(
            pstmt->executeQuery()
        );

        if(res->next())
        {
            std::optional<std::string>
                description;

            if(
                !res->isNull(
                    "description"
                )
            )
            {
                description =
                    res->getString(
                        "description"
                    );
            }

            Goal g(
                res->getInt(
                    "goal_id"
                ),
                description
            );

            return g;
        }

        return std::nullopt;
    }
    catch(sql::SQLException& e)
    {
        db_logger->error(
            "Goal getById failed: {}",
            e.what()
        );

        throw;
    }
}