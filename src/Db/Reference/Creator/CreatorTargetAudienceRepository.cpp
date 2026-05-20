#include "Db/Reference/Creator/CreatorTargetAudienceRepository.h"

#include <memory>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

CreatorTargetAudienceRepository::CreatorTargetAudienceRepository(DbManager& db)
    : dbManager(db) {}

void CreatorTargetAudienceRepository::insert(int creatorId, int audId)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "INSERT INTO CREATOR_TARGET_AUDIENCE (creator_id, aud_id, is_deleted) "
            "VALUES (?, ?, 0) ON DUPLICATE KEY UPDATE is_deleted = 0"
        )
    );

    stmt->setInt(1, creatorId);
    stmt->setInt(2, audId);
    stmt->execute();
}

std::vector<int> CreatorTargetAudienceRepository::getByCreatorId(int creatorId)
{
    std::vector<int> result;
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "SELECT aud_id FROM CREATOR_TARGET_AUDIENCE "
            "WHERE creator_id = ? AND is_deleted = 0"
        )
    );

    stmt->setInt(1, creatorId);

    auto res = std::unique_ptr<sql::ResultSet>(stmt->executeQuery());

    while (res->next())
    {
        result.push_back(res->getInt("aud_id"));
    }

    return result;
}

void CreatorTargetAudienceRepository::softDelete(int creatorId, int audId)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "UPDATE CREATOR_TARGET_AUDIENCE SET is_deleted = 1 "
            "WHERE creator_id = ? AND aud_id = ?"
        )
    );

    stmt->setInt(1, creatorId);
    stmt->setInt(2, audId);
    stmt->execute();
}