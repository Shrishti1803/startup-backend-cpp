#include "Db/Creator/ProfileAccountRepository.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>



ProfileAccountRepository::ProfileAccountRepository(DbManager& db)
    : dbManager(db)
{}


/* =====================================================
   CREATE
===================================================== */

int ProfileAccountRepository::insert(
    sql::Connection* conn,
    int creator_id,
    int platform_id,
    const std::string& link,
    const std::optional<long long>& followers)
{
    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "INSERT INTO PROFILE_ACCOUNT "
            "(creator_id, platform_id, link, followers) "
            "VALUES (?, ?, ?, ?)"
        )
    );

    pstmt->setInt(1, creator_id);

    pstmt->setInt(2, platform_id);

    pstmt->setString(3, link);

    if (followers)
        pstmt->setInt64(4, followers.value());
    else
        pstmt->setNull(4, sql::DataType::BIGINT);

    pstmt->executeUpdate();


    std::unique_ptr<sql::Statement> stmt(
        conn->createStatement()
    );

    std::unique_ptr<sql::ResultSet> rs(
        stmt->executeQuery(
            "SELECT LAST_INSERT_ID()"
        )
    );

    if (rs->next())
        return rs->getInt(1);

    return -1;
}


/* =====================================================
   READ BY ID
===================================================== */

ProfileAccount
ProfileAccountRepository::getById(int profile_id)
{

    auto conn = dbManager.getConnection();

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "SELECT "
            "profile_id, "
            "creator_id, "
            "platform_id, "
            "link, "
            "followers "
            "FROM PROFILE_ACCOUNT "
            "WHERE profile_id=? "
            "AND is_deleted=0"
        )
    );

    pstmt->setInt(1, profile_id);

    std::unique_ptr<sql::ResultSet> rs(
        pstmt->executeQuery()
    );

    if (!rs->next())
    {
        throw std::runtime_error(
            "ProfileAccount not found"
        );
    }

    std::optional<long long> followers;

    if(!rs->isNull("followers"))
    {
        followers =
            rs->getInt64("followers");
    }

    return ProfileAccount(
        rs->getInt("profile_id"),
        rs->getInt("creator_id"),
        rs->getInt("platform_id"),
        rs->getString("link"),
        followers
    );
}

/* =====================================================
   READ BY CREATOR ID
===================================================== */

std::vector<ProfileAccount>
ProfileAccountRepository::getByCreatorId(
    int creator_id
)
{

    auto conn = dbManager.getConnection();

    std::vector<ProfileAccount> accounts;

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "SELECT "
            "profile_id, "
            "creator_id, "
            "platform_id, "
            "link, "
            "followers "
            "FROM PROFILE_ACCOUNT "
            "WHERE creator_id=? "
            "AND is_deleted=0"
        )
    );

    pstmt->setInt(1, creator_id);

    std::unique_ptr<sql::ResultSet> rs(
        pstmt->executeQuery()
    );

    while(rs->next())
    {

        std::optional<long long> followers;

        if(!rs->isNull("followers"))
        {
            followers =
                rs->getInt64("followers");
        }

        accounts.emplace_back(
            rs->getInt("profile_id"),
            rs->getInt("creator_id"),
            rs->getInt("platform_id"),
            rs->getString("link"),
            followers
        );
    }

    return accounts;
}

/* =====================================================
   READ ALL
===================================================== */

std::vector<ProfileAccount>
ProfileAccountRepository::getAll()
{

    auto conn = dbManager.getConnection();

    std::vector<ProfileAccount> accounts;

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "SELECT "
            "profile_id, "
            "creator_id, "
            "platform_id, "
            "link, "
            "followers "
            "FROM PROFILE_ACCOUNT "
            "WHERE is_deleted=0"
        )
    );

    std::unique_ptr<sql::ResultSet> rs(
        pstmt->executeQuery()
    );

    while(rs->next())
    {

        std::optional<long long> followers;

        if(!rs->isNull("followers"))
        {
            followers =
                rs->getInt64("followers");
        }

        accounts.emplace_back(
            rs->getInt("profile_id"),
            rs->getInt("creator_id"),
            rs->getInt("platform_id"),
            rs->getString("link"),
            followers
        );
    }

    return accounts;
}

/* =====================================================
   PATCH UPDATE
===================================================== */

void ProfileAccountRepository::update(
    sql::Connection* conn,
    int profile_id,
    const std::optional<int>& platform_id,
    const std::optional<std::string>& link,
    const std::optional<long long>& followers)
{


    std::string query =
        "UPDATE PROFILE_ACCOUNT SET ";

    std::vector<std::string> fields;

    if(platform_id)
    {
        fields.push_back(
            "platform_id=?"
        );
    }

    if(link)
    {
        fields.push_back(
            "link=?"
        );
    }

    if(followers)
    {
        fields.push_back(
            "followers=?"
        );
    }

    for(size_t i = 0; i < fields.size(); i++)
    {
        query += fields[i];

        if(i < fields.size() - 1)
        {
            query += ", ";
        }
    }

    query += " WHERE profile_id=?";

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(query)
    );

    int index = 1;

    if(platform_id)
    {
        pstmt->setInt(
            index++,
            platform_id.value()
        );
    }

    if(link)
    {
        pstmt->setString(
            index++,
            link.value()
        );
    }

    if(followers)
    {
        pstmt->setInt64(
            index++,
            followers.value()
        );
    }

    pstmt->setInt(
        index,
        profile_id
    );

    pstmt->executeUpdate();
}


/* =====================================================
   SOFT DELETE
===================================================== */
void ProfileAccountRepository::softDelete(
    sql::Connection* conn,
    int profile_id)
{


    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "UPDATE PROFILE_ACCOUNT SET is_deleted=1 WHERE profile_id=?"
        )
    );

    pstmt->setInt(1, profile_id);

    pstmt->executeUpdate();
}