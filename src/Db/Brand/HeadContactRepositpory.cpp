#include "Db/Brand/HeadContactRepository.h"
#include "Utils/Logger.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

HeadContactRepository::HeadContactRepository(DbManager& db)
    : dbManager(db) {}

int HeadContactRepository::insert(
    sql::Connection* conn,
    int headId,
    const std::optional<std::string>& contactType,
    const std::string& contactValue)
{
    auto db_logger = Logger::db();
    db_logger->info("Inserting head contact for head_id={}", headId);

    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "INSERT INTO HEAD_CONTACT "
                "(head_id, contact_type, contact_value) "
                "VALUES (?, ?, ?)"
            )
        );

        pstmt->setInt(1, headId);

        if (contactType.has_value())
            pstmt->setString(2, contactType.value());
        else
            pstmt->setNull(2, sql::DataType::VARCHAR);

        pstmt->setString(3, contactValue);

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
        db_logger->error("HeadContact insert failed: {}", e.what());
        throw;
    }
}

std::vector<HeadContact>
HeadContactRepository::getByHeadId(int headId)
{
    auto db_logger = Logger::db();
    db_logger->info("Fetching contacts for head_id={}", headId);

    std::vector<HeadContact> contacts;

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "SELECT contacts_id, contact_type, contact_value "
                "FROM HEAD_CONTACT "
                "WHERE head_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setInt(1, headId);

        std::unique_ptr<sql::ResultSet> rs(pstmt->executeQuery());

        while (rs->next()) {

            std::optional<std::string> type;

            if (!rs->isNull("contact_type")) {
                type = std::string(rs->getString("contact_type"));
            }

            HeadContact contact(
                rs->getInt("contacts_id"),
                std::string(rs->getString("contact_value")),
                type
            );

            contacts.push_back(contact);
        }
    }
    catch (sql::SQLException& e) {
        db_logger->error("HeadContact fetch failed: {}", e.what());
        throw;
    }

    return contacts;
}

void HeadContactRepository::softDelete(
    sql::Connection* conn,
    int contactId)
{
    auto db_logger = Logger::db();
    db_logger->info("Soft deleting contact_id={}", contactId);

    try {

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "UPDATE HEAD_CONTACT "
                "SET is_deleted = 1 "
                "WHERE contact_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setInt(1, contactId);

        int rowsAffected = pstmt->executeUpdate();

        if (rowsAffected == 0) {
            db_logger->warn("No head contact found to delete with ID {}", contactId);
            throw std::runtime_error("HeadContact not found for deletion");
        }

        db_logger->info("HeadContact {} soft deleted successfully", contactId);
    }
    catch (sql::SQLException& e) {
        db_logger->error("HeadContact delete failed: {}", e.what());
        throw;
    }
}


void HeadContactRepository::update(
    sql::Connection* conn,
    int contactId,
    const std::optional<std::string>& contactType,
    const std::optional<std::string>& contactValue)
{
    auto db_logger = Logger::db();
    db_logger->info("Updating contact_id={}", contactId);

   
    if (!contactType.has_value() && !contactValue.has_value()) {
        db_logger->warn("No fields provided to update for contact_id={}", contactId);
        return;
    }

    try {

        std::string query = "UPDATE HEAD_CONTACT SET ";
        std::vector<std::string> fields;

        if (contactType.has_value())
            fields.push_back("contact_type = ?");

        if (contactValue.has_value())
            fields.push_back("contact_value = ?");

        // Build dynamic SET clause
        for (size_t i = 0; i < fields.size(); ++i) {
            query += fields[i];
            if (i < fields.size() - 1)
                query += ", ";
        }

        query += " WHERE contacts_id = ? AND is_deleted = 0";

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(query)
        );

        int paramIndex = 1;

        if (contactType.has_value())
            pstmt->setString(paramIndex++, contactType.value());

        if (contactValue.has_value())
            pstmt->setString(paramIndex++, contactValue.value());

        pstmt->setInt(paramIndex, contactId);

        int rowsAffected = pstmt->executeUpdate();

        if (rowsAffected == 0) {
            db_logger->warn("No contact found to update with ID {}", contactId);
            throw std::runtime_error("HeadContact not found for update");
        }

        db_logger->info("Contact {} updated successfully", contactId);
    }
    catch (sql::SQLException& e) {
        db_logger->error("HeadContact update failed: {}", e.what());
        throw;
    }
}

std::optional<HeadContact>
HeadContactRepository::getById(int contactId)
{
    auto db_logger = Logger::db();

    db_logger->info(
        "Fetching head contact by id={}",
        contactId
    );

    try
    {
        auto conn =
            dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                R"(
                    SELECT
                        contact_id,
                        contact_type,
                        contact_value
                    FROM HEAD_CONTACT
                    WHERE contact_id = ?
                    AND is_deleted = 0
                )"
            )
        );

        pstmt->setInt(1, contactId);

        std::unique_ptr<sql::ResultSet> res(
            pstmt->executeQuery()
        );

        if(res->next())
        {
            HeadContact c;

            c.setId(
                res->getInt(
                    "contact_id"
                )
            );

            if(
                !res->isNull(
                    "contact_type"
                )
            )
            {
                c.setType(
                    std::string(
                        res->getString(
                            "contact_type"
                        )
                    )
                );
            }

            c.setValue(
                std::string(
                    res->getString(
                        "contact_value"
                    )
                )
            );

            return c;
        }

        return std::nullopt;
    }
    catch(sql::SQLException& e)
    {
        db_logger->error(
            "HeadContact getById failed: {}",
            e.what()
        );

        throw;
    }
}