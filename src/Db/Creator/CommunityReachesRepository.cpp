#include "Db/Creator/CommunityReachesRepository.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

CommunityReachesRepository::CommunityReachesRepository(DbManager& db)
    : dbManager(db) {}

int CommunityReachesRepository::insert(
    sql::Connection* conn,
    int creatorId,
    const std::optional<std::string>& sentiment,
    const std::optional<std::string>& replyRate)
{

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "INSERT INTO COMMUNITY_REACHES (creator_id, sentiment, reply_rate) VALUES (?, ?, ?)"
        )
    );

    pstmt->setInt(1, creatorId);

    if (sentiment) pstmt->setString(2, sentiment.value());
    else pstmt->setNull(2, sql::DataType::VARCHAR);

    if (replyRate) pstmt->setString(3, replyRate.value());
    else pstmt->setNull(3, sql::DataType::VARCHAR);

    pstmt->executeUpdate();

    std::unique_ptr<sql::Statement> stmt(conn->createStatement());
    std::unique_ptr<sql::ResultSet> rs(stmt->executeQuery("SELECT LAST_INSERT_ID()"));

    if (rs->next())
        return rs->getInt(1);

    return -1;
}

std::optional<CommunityReaches>
CommunityReachesRepository::getByCreatorId(int creatorId)
{
    auto conn = dbManager.getConnection();

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "SELECT comm_id, sentiment, reply_rate "
            "FROM COMMUNITY_REACHES WHERE creator_id=? AND is_deleted=0"
        )
    );

    pstmt->setInt(1, creatorId);

    std::unique_ptr<sql::ResultSet> rs(pstmt->executeQuery());

    if (!rs->next())
        return std::nullopt;

    std::optional<std::string> sentiment;
    std::optional<std::string> replyRate;

    if (!rs->isNull("sentiment"))
        sentiment = rs->getString("sentiment");

    if (!rs->isNull("reply_rate"))
        replyRate = rs->getString("reply_rate");

    return CommunityReaches(
        rs->getInt("comm_id"),
        sentiment,
        replyRate
    );
}

void CommunityReachesRepository::update(
    sql::Connection* conn,
    int commId,
    const std::optional<std::string>& sentiment,
    const std::optional<std::string>& replyRate)
{

    std::string query = "UPDATE COMMUNITY_REACHES SET ";
    std::vector<std::string> fields;

    if (sentiment) fields.push_back("sentiment=?");
    if (replyRate) fields.push_back("reply_rate=?");

    for (size_t i = 0; i < fields.size(); i++) {
        query += fields[i];
        if (i < fields.size() - 1)
            query += ", ";
    }

    query += " WHERE comm_id=?";

    std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(query));

    int index = 1;

    if (sentiment) pstmt->setString(index++, sentiment.value());
    if (replyRate) pstmt->setString(index++, replyRate.value());

    pstmt->setInt(index, commId);

    pstmt->executeUpdate();
}

void CommunityReachesRepository::softDelete(
    sql::Connection* conn,
    int commId)
{
    

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "UPDATE COMMUNITY_REACHES SET is_deleted=1 WHERE comm_id=?"
        )
    );

    pstmt->setInt(1, commId);

    pstmt->executeUpdate();
}