#include "Db/Creator/VideoL10Repository.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
VideoL10Repository::VideoL10Repository(DbManager& db)
    : dbManager(db) {}

int VideoL10Repository::insert(
    sql::Connection* conn,
    int creatorId,
    const std::optional<std::string>& consistency,
    const std::optional<std::string>& avgViews,
    const std::optional<std::string>& avgER,
    const std::optional<std::string>& sharability,
    const std::optional<std::string>& velocityTrend)
{

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "INSERT INTO VIDEO_L10 "
            "(creator_id, L10_consistency, L10_avg_views, L10_avg_ER, "
            "L10_sharability, velocity_trend) "
            "VALUES (?, ?, ?, ?, ?, ?)"
        )
    );

    pstmt->setInt(1, creatorId);

    if (consistency) pstmt->setString(2, consistency.value());
    else pstmt->setNull(2, sql::DataType::VARCHAR);

    if (avgViews) pstmt->setString(3, avgViews.value());
    else pstmt->setNull(3, sql::DataType::VARCHAR);

    if (avgER) pstmt->setString(4, avgER.value());
    else pstmt->setNull(4, sql::DataType::VARCHAR);

    if (sharability) pstmt->setString(5, sharability.value());
    else pstmt->setNull(5, sql::DataType::VARCHAR);

    if (velocityTrend) pstmt->setString(6, velocityTrend.value());
    else pstmt->setNull(6, sql::DataType::VARCHAR);
    pstmt->executeUpdate();

    std::unique_ptr<sql::Statement> stmt(
        conn->createStatement()
    );

    std::unique_ptr<sql::ResultSet> rs(
        stmt->executeQuery(
            "SELECT LAST_INSERT_ID()"
        )
    );

    if(rs->next())
    {
        return rs->getInt(1);
    }

    return -1;
}

std::vector<VideoL10>
VideoL10Repository::getByCreatorId(int creatorId)
{
    auto conn = dbManager.getConnection();

    std::vector<VideoL10> videos;

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "SELECT performance_id, L10_consistency, L10_avg_views, "
            "L10_avg_ER, L10_sharability, velocity_trend "
            "FROM VIDEO_L10 WHERE creator_id=? AND is_deleted=0"
        )
    );

    pstmt->setInt(1, creatorId);

    std::unique_ptr<sql::ResultSet> rs(pstmt->executeQuery());

    while (rs->next()) {

        videos.emplace_back(
            rs->getInt("performance_id"),
            rs->getString("L10_consistency"),
            rs->getString("L10_avg_views"),
            rs->getString("L10_avg_ER"),
            rs->getString("L10_sharability"),
            rs->getString("velocity_trend")
        );
    }

    return videos;
}

void VideoL10Repository::update(
    sql::Connection* conn,
    int performanceId,
    const std::optional<std::string>& consistency,
    const std::optional<std::string>& avgViews,
    const std::optional<std::string>& avgER,
    const std::optional<std::string>& sharability,
    const std::optional<std::string>& velocityTrend)
{
    std::string query = "UPDATE VIDEO_L10 SET ";
    std::vector<std::string> fields;

    if (consistency) fields.push_back("L10_consistency=?");
    if (avgViews) fields.push_back("L10_avg_views=?");
    if (avgER) fields.push_back("L10_avg_ER=?");
    if (sharability) fields.push_back("L10_sharability=?");
    if (velocityTrend) fields.push_back("velocity_trend=?");

    for (size_t i = 0; i < fields.size(); i++) {
        query += fields[i];
        if (i < fields.size() - 1)
            query += ", ";
    }

    query += " WHERE performance_id=?";

    std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(query));

    int index = 1;

    if (consistency) pstmt->setString(index++, consistency.value());
    if (avgViews) pstmt->setString(index++, avgViews.value());
    if (avgER) pstmt->setString(index++, avgER.value());
    if (sharability) pstmt->setString(index++, sharability.value());
    if (velocityTrend) pstmt->setString(index++, velocityTrend.value());

    pstmt->setInt(index, performanceId);

    pstmt->executeUpdate();
}

void VideoL10Repository::softDelete(
    sql::Connection* conn,
    int performanceId)
{
    

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "UPDATE VIDEO_L10 SET is_deleted=1 WHERE performance_id=?"
        )
    );

    pstmt->setInt(1, performanceId);

    pstmt->executeUpdate();
}

std::optional<VideoL10>
VideoL10Repository::getById(
    int performanceId
)
{
    auto conn =
        dbManager.getConnection();

    std::unique_ptr<
        sql::PreparedStatement
    > pstmt(
        conn->prepareStatement(
            "SELECT performance_id, "
            "L10_consistency, "
            "L10_avg_views, "
            "L10_avg_ER, "
            "L10_sharability, "
            "velocity_trend "
            "FROM VIDEO_L10 "
            "WHERE performance_id=? "
            "AND is_deleted=0"
        )
    );

    pstmt->setInt(
        1,
        performanceId
    );

    std::unique_ptr<
        sql::ResultSet
    > rs(
        pstmt->executeQuery()
    );

    if (!rs->next())
    {
        return std::nullopt;
    }

    std::optional<std::string>
        consistency;

    std::optional<std::string>
        avgViews;

    std::optional<std::string>
        avgER;

    std::optional<std::string>
        sharability;

    std::optional<std::string>
        trend;

    if (
        !rs->isNull(
            "L10_consistency"
        )
    )
    {
        consistency =
            rs->getString(
                "L10_consistency"
            );
    }

    if (
        !rs->isNull(
            "L10_avg_views"
        )
    )
    {
        avgViews =
            rs->getString(
                "L10_avg_views"
            );
    }

    if (
        !rs->isNull(
            "L10_avg_ER"
        )
    )
    {
        avgER =
            rs->getString(
                "L10_avg_ER"
            );
    }

    if (
        !rs->isNull(
            "L10_sharability"
        )
    )
    {
        sharability =
            rs->getString(
                "L10_sharability"
            );
    }

    if (
        !rs->isNull(
            "velocity_trend"
        )
    )
    {
        trend =
            rs->getString(
                "velocity_trend"
            );
    }

    return VideoL10(
        rs->getInt(
            "performance_id"
        ),
        consistency,
        avgViews,
        avgER,
        sharability,
        trend
    );
}