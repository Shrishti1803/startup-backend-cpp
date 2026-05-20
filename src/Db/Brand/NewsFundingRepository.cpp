#include "Db/Brand/NewsFundingRepository.h"
#include "Utils/Logger.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>

#include <memory>
#include <vector>
#include <string>

NewsFundingRepository::NewsFundingRepository(DbManager& db)
    : dbManager(db) {}

int NewsFundingRepository::insert(
    sql::Connection* conn,
    int brandId,
    const std::optional<std::string>& eventType,
    const std::optional<std::string>& title,
    const std::optional<std::string>& description,
    const std::optional<Date>& eventDate)
{
    auto db_logger = Logger::db();
    db_logger->info("Inserting news/funding for brand_id={}", brandId);

    try {

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "INSERT INTO NEWS_FUNDING "
                "(brand_id, event_type, title, description, event_date) "
                "VALUES (?, ?, ?, ?, ?)"
            )
        );

        pstmt->setInt(1, brandId);

        if (eventType)
            pstmt->setString(2, eventType.value());
        else
            pstmt->setNull(2, sql::DataType::VARCHAR);

        if (title)
            pstmt->setString(3, title.value());
        else
            pstmt->setNull(3, sql::DataType::VARCHAR);

        if (description)
            pstmt->setString(4, description.value());
        else
            pstmt->setNull(4, sql::DataType::VARCHAR);

        if (eventDate)
            pstmt->setString(5, eventDate->toString()); // must match YYYY-MM-DD
        else
            pstmt->setNull(5, sql::DataType::DATE);

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
        db_logger->error("NewsFunding insert failed: {}", e.what());
        throw;
    }
}

std::vector<NewsFunding>
NewsFundingRepository::getByBrandId(int brandId)
{
    auto db_logger = Logger::db();
    db_logger->info("Fetching news/funding for brand_id={}", brandId);

    std::vector<NewsFunding> records;

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "SELECT news_id, event_type, title, description, event_date "
                "FROM NEWS_FUNDING "
                "WHERE brand_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setInt(1, brandId);

        std::unique_ptr<sql::ResultSet> rs(pstmt->executeQuery());

        while (rs->next()) {

            std::optional<std::string> eventType, title, description;
            std::optional<Date> eventDate;

            if (!rs->isNull("event_type"))
                eventType = rs->getString("event_type");

            if (!rs->isNull("title"))
                title = rs->getString("title");

            if (!rs->isNull("description"))
                description = rs->getString("description");

            if (!rs->isNull("event_date"))
                eventDate = Date::fromString(rs->getString("event_date"));

            NewsFunding record(
                rs->getInt("news_id"),
                eventType,
                title,
                description,
                eventDate
            );

            records.push_back(record);
        }
    }
    catch (sql::SQLException& e) {
        db_logger->error("NewsFunding fetch failed: {}", e.what());
        throw;
    }

    return records;
}

void NewsFundingRepository::update(
    sql::Connection* conn,
    int newsId,
    const std::optional<std::string>& eventType,
    const std::optional<std::string>& title,
    const std::optional<std::string>& description,
    const std::optional<Date>& eventDate)
{
    auto db_logger = Logger::db();
    db_logger->info("Updating news_id={}", newsId);

    if (!eventType && !title && !description && !eventDate) {
        db_logger->warn("No fields provided for news update id={}", newsId);
        return;
    }

    try {

        std::string query = "UPDATE NEWS_FUNDING SET ";
        std::vector<std::string> fields;

        if (eventType) fields.push_back("event_type = ?");
        if (title) fields.push_back("title = ?");
        if (description) fields.push_back("description = ?");
        if (eventDate) fields.push_back("event_date = ?");

        for (size_t i = 0; i < fields.size(); ++i) {
            query += fields[i];
            if (i < fields.size() - 1)
                query += ", ";
        }

        query += " WHERE news_id = ? AND is_deleted = 0";

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(query)
        );

        int index = 1;

        if (eventType) pstmt->setString(index++, eventType.value());
        if (title) pstmt->setString(index++, title.value());
        if (description) pstmt->setString(index++, description.value());
        if (eventDate) pstmt->setString(index++, eventDate->toString());

        pstmt->setInt(index, newsId);

        int rows = pstmt->executeUpdate();

        if (rows == 0)
            throw std::runtime_error("NewsFunding not found for update");

        db_logger->info("NewsFunding {} updated successfully", newsId);
    }
    catch (sql::SQLException& e) {
        db_logger->error("NewsFunding update failed: {}", e.what());
        throw;
    }
}

void NewsFundingRepository::softDelete(
    [[maybe_unused]] sql::Connection* conn,
    int newsId)
{
    auto db_logger = Logger::db();
    db_logger->info("Soft deleting news_id={}", newsId);

    try {
        auto conn = dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "UPDATE NEWS_FUNDING SET is_deleted = 1 "
                "WHERE news_id = ? AND is_deleted = 0"
            )
        );

        pstmt->setInt(1, newsId);

        int rows = pstmt->executeUpdate();

        if (rows == 0)
            throw std::runtime_error("NewsFunding not found for deletion");

        db_logger->info("NewsFunding {} soft deleted", newsId);
    }
    catch (sql::SQLException& e) {
        db_logger->error("NewsFunding delete failed: {}", e.what());
        throw;
    }
}

std::optional<NewsFunding>
NewsFundingRepository::getById(
    int newsFundingId
)
{
    auto db_logger = Logger::db();

    db_logger->info(
        "Fetching news funding by id={}",
        newsFundingId
    );

    try
    {
        auto conn =
            dbManager.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                R"(
                    SELECT
                        news_id,
                        event_type,
                        title,
                        description,
                        event_date
                    FROM NEWS_FUNDING
                    WHERE news_id = ?
                    AND is_deleted = 0
                )"
            )
        );

        pstmt->setInt(
            1,
            newsFundingId
        );

        std::unique_ptr<sql::ResultSet> res(
            pstmt->executeQuery()
        );

        if(res->next())
        {
            std::optional<std::string>
                eventType,
                title,
                description;

            std::optional<Date> date;

            if(!res->isNull("event_type"))
            {
                eventType =
                    res->getString(
                        "event_type"
                    );
            }

            if(!res->isNull("title"))
            {
                title =
                    res->getString(
                        "title"
                    );
            }

            if(!res->isNull("description"))
            {
                description =
                    res->getString(
                        "description"
                    );
            }

            if(!res->isNull("event_date"))
            {
                date =
                    Date::fromString(
                        res->getString(
                            "event_date"
                        )
                    );
            }

            NewsFunding n(
                res->getInt(
                    "news_id"
                ),
                eventType,
                title,
                description,
                date
            );

            return n;
        }

        return std::nullopt;
    }
    catch(sql::SQLException& e)
    {
        db_logger->error(
            "NewsFunding getById failed: {}",
            e.what()
        );

        throw;
    }
}