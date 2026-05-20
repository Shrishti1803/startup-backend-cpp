#include "Db/Creator/CreatorRepository.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

CreatorRepository::CreatorRepository(DbManager& db)
    : dbManager(db) {}

int CreatorRepository::insert(
    sql::Connection* conn,
    const std::string& name,
    const std::optional<std::string>& email,
    const std::optional<std::string>& rating,
    const std::optional<long long>& audienceSize)
{
    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "INSERT INTO CREATOR (creator_name, email_id, rating, audience_size) VALUES (?, ?, ?, ?)"
        )
    );

    pstmt->setString(1, name);

    if (email) pstmt->setString(2, email.value());
    else pstmt->setNull(2, sql::DataType::VARCHAR);

    if (rating) pstmt->setString(3, rating.value());
    else pstmt->setNull(3, sql::DataType::VARCHAR);
    
    if (audienceSize)
        pstmt->setInt64(4, audienceSize.value());
    else
        pstmt->setNull(4, sql::DataType::BIGINT);

    pstmt->executeUpdate();

    std::unique_ptr<sql::Statement> stmt(conn->createStatement());
    std::unique_ptr<sql::ResultSet> rs(stmt->executeQuery("SELECT LAST_INSERT_ID()"));

    if (rs->next())
        return rs->getInt(1);

    return -1;
}

Creator CreatorRepository::getById(int creatorId)
{
    auto conn = dbManager.getConnection();

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "SELECT creator_id, creator_name, email_id, rating, audience_size "
            "FROM CREATOR WHERE creator_id=? AND is_deleted=0"
        )
    );

    pstmt->setInt(1, creatorId);

    std::unique_ptr<sql::ResultSet> rs(pstmt->executeQuery());

    if (!rs->next())
        throw std::runtime_error("Creator not found");

    std::optional<std::string> email;
    std::optional<std::string> rating;
    std::optional<long long> audience;

    if (!rs->isNull("email_id"))
        email = rs->getString("email_id");

    if (!rs->isNull("rating"))
        rating = rs->getString("rating");

    if (!rs->isNull("audience_size"))
    audience = rs->getInt64("audience_size");

    return Creator(
        rs->getInt("creator_id"),
        rs->getString("creator_name"),
        email,
        rating,
        audience
    );
}

std::vector<Creator> CreatorRepository::getAll()
{
    auto conn = dbManager.getConnection();

    std::vector<Creator> creators;

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "SELECT creator_id, creator_name, audience_size FROM CREATOR WHERE is_deleted=0"
        )
    );

    std::unique_ptr<sql::ResultSet> rs(pstmt->executeQuery());

    while (rs->next()) {
        std::optional<long long> audience;

        if (!rs->isNull("audience_size"))
            audience = rs->getInt64("audience_size");

        creators.emplace_back(
            rs->getInt("creator_id"),
            rs->getString("creator_name"),
            std::nullopt,
            std::nullopt,
            audience
        );
    }

    return creators;
}


//UPDATE FUNCTIONS FOR DIFFERENT FIELDS
void CreatorRepository::updateName(
    sql::Connection* conn,
    int creatorId,
    const std::optional<std::string>& name)
{
    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "UPDATE CREATOR SET creator_name=? WHERE creator_id=?"
        )
    );

    if(name)
        pstmt->setString(1, name.value());
    else
        pstmt->setNull(1, sql::DataType::VARCHAR);

    pstmt->setInt(2, creatorId);

    pstmt->executeUpdate();
}


void CreatorRepository::updateEmail(
    sql::Connection* conn,
    int creatorId,
    const std::optional<std::string>& email)
{
    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "UPDATE CREATOR SET email_id=? WHERE creator_id=?"
        )
    );

    if(email)
        pstmt->setString(1, email.value());
    else
        pstmt->setNull(1, sql::DataType::VARCHAR);

    pstmt->setInt(2, creatorId);

    pstmt->executeUpdate();
}


void CreatorRepository::updateRating(
    sql::Connection* conn,
    int creatorId,
    const std::optional<std::string>& rating)
{
    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "UPDATE CREATOR SET rating=? WHERE creator_id=?"
        )
    );

    if(rating)
        pstmt->setString(1, rating.value());
    else
        pstmt->setNull(1, sql::DataType::VARCHAR);

    pstmt->setInt(2, creatorId);

    pstmt->executeUpdate();
}

void CreatorRepository::updateAudienceSize(
    sql::Connection* conn,
    int creatorId,
    const std::optional<long long>& audienceSize)
{
    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "UPDATE CREATOR SET audience_size=? WHERE creator_id=?"
        )
    );

    if(audienceSize)
        pstmt->setInt64(1, audienceSize.value());
    else
        pstmt->setNull(1, sql::DataType::BIGINT);

    pstmt->setInt(2, creatorId);

    pstmt->executeUpdate();
}












// end of update functions 

void CreatorRepository::softDelete(
    sql::Connection* conn,
    int creatorId)
{
    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "UPDATE CREATOR SET is_deleted=1 WHERE creator_id=?"
        )
    );

    pstmt->setInt(1, creatorId);

    pstmt->executeUpdate();
}

bool CreatorRepository::existsById(sql::Connection* conn, int creatorId)
{
    std::unique_ptr<sql::PreparedStatement> stmt(
        conn->prepareStatement(
            "SELECT 1 FROM CREATOR WHERE creator_id = ? AND is_deleted = 0 LIMIT 1"
        )
    );

    stmt->setInt(1, creatorId);

    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

    return res->next();
}