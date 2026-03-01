#include "Db/Brand/HeadRepository.h"
#include "Utils/Logger.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>

#include <memory>
#include <vector>
#include <string>

HeadRepository::HeadRepository(DbManager& db)
    : dbManager(db) {}

int HeadRepository::insert(int brandId,
                           int positionId,
                           const std::string& name,
                           const std::optional<std::string>& linkedinId,
                           const std::string& personalMail,
                           const std::string& officialMail)
{
    auto db_logger = Logger::db();
    db_logger->info("Inserting head for brand_id={}", brandId);

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "INSERT INTO HEADS "
                "(brand_id, position_id, name, linkedin_id, personal_mail, official_mail) "
                "VALUES (?, ?, ?, ?, ?, ?)"
            )
        );

        pstmt->setInt(1, brandId);
        pstmt->setInt(2, positionId);
        pstmt->setString(3, name);

        if (linkedinId.has_value())
            pstmt->setString(4, linkedinId.value());
        else
            pstmt->setNull(4, sql::DataType::VARCHAR);

        pstmt->setString(5, personalMail);
        pstmt->setString(6, officialMail);

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
        db_logger->error("Head insert failed: {}", e.what());
        throw;
    }
}

std::vector<Head> HeadRepository::getByBrandId(int brandId)
{
    auto db_logger = Logger::db();
    db_logger->info("Fetching heads for brand_id={}", brandId);

    std::vector<Head> heads;

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "SELECT h.head_id, h.position_id, h.name, "
                "h.linkedin_id, h.personal_mail, h.official_mail, "
                "p.position_name "
                "FROM HEADS h "
                "JOIN B_POSITION p ON h.position_id = p.position_id "
                "WHERE h.brand_id = ? AND h.is_deleted = 0"
            )
        );

        pstmt->setInt(1, brandId);

        std::unique_ptr<sql::ResultSet> rs(pstmt->executeQuery());

        while (rs->next()) {

            Position pos(
                rs->getInt("position_id"),
                rs->getString("position_name")
            );

            std::optional<std::string> linkedin;
            if (!rs->isNull("linkedin_id"))
                linkedin = std::string(rs->getString("linkedin_id"));

            Head head(
                rs->getInt("head_id"),
                pos,
                rs->getString("name"),
                rs->getString("personal_mail"),
                rs->getString("official_mail"),
                linkedin
            );

            heads.push_back(head);
        }
    }
    catch (sql::SQLException& e) {
        db_logger->error("Head fetch failed: {}", e.what());
        throw;
    }

    return heads;
}

void HeadRepository::update(
        int headId,
        const std::optional<int>& positionId,
        const std::optional<std::string>& name,
        const std::optional<std::string>& linkedinId,
        const std::optional<std::string>& personalMail,
        const std::optional<std::string>& officialMail)
{
    auto db_logger = Logger::db();
    db_logger->info("Updating head_id={}", headId);

    if (!positionId && !name && !linkedinId && !personalMail && !officialMail) {
        db_logger->warn("No fields provided for head update id={}", headId);
        return;
    }

    try {
        auto conn = dbManager.getConnection();

        std::string query = "UPDATE HEADS SET ";
        std::vector<std::string> fields;

        if (positionId) fields.push_back("position_id = ?");
        if (name) fields.push_back("name = ?");
        if (linkedinId) fields.push_back("linkedin_id = ?");
        if (personalMail) fields.push_back("personal_mail = ?");
        if (officialMail) fields.push_back("official_mail = ?");

        for (size_t i = 0; i < fields.size(); ++i) {
            query += fields[i];
            if (i < fields.size() - 1)
                query += ", ";
        }

        query += " WHERE head_id = ? AND is_deleted = 0";

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(query)
        );

        int index = 1;

        if (positionId) pstmt->setInt(index++, positionId.value());
        if (name) pstmt->setString(index++, name.value());

        if (linkedinId)
            pstmt->setString(index++, linkedinId.value());

        if (personalMail)
            pstmt->setString(index++, personalMail.value());

        if (officialMail)
            pstmt->setString(index++, officialMail.value());

        pstmt->setInt(index, headId);

        int rows = pstmt->executeUpdate();

        if (rows == 0){
            db_logger->warn("No head found to update with id {}", headId);
            throw std::runtime_error("Head not found for update");
        }
        db_logger->info("Head {} updated successfully", headId);
    }
    catch (sql::SQLException& e) {
        db_logger->error("Head update failed: {}", e.what());
        throw;
    }
}

void HeadRepository::softDelete(int headId)
{
    auto db_logger = Logger::db();
    db_logger->info("Soft deleting head_id={}", headId);

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "UPDATE HEADS SET is_deleted = 1 "
                "WHERE head_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setInt(1, headId);

        int rows = pstmt->executeUpdate();

        if (rows == 0)
            throw std::runtime_error("Head not found for deletion");

        db_logger->info("Head {} soft deleted", headId);
    }
    catch (sql::SQLException& e) {
        db_logger->error("Head delete failed: {}", e.what());
        throw;
    }
}