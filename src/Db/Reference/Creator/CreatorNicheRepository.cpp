#include "Db/Reference/Creator/CreatorNicheRepository.h"

#include <memory>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

CreatorNicheRepository::CreatorNicheRepository(DbManager& db)
    : dbManager(db) {}

void CreatorNicheRepository::insert(int creatorId, int nicheId)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement("INSERT INTO CREATOR_NICHE (creator_id, niche_id, is_deleted) VALUES (?, ?, 0) ON DUPLICATE KEY UPDATE is_deleted = 0")
    );

    stmt->setInt(1, creatorId);
    stmt->setInt(2, nicheId);
    stmt->execute();
}

std::vector<Niche> CreatorNicheRepository::getByCreatorId(int creatorId)
{
    std::vector<Niche> result;
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "SELECT n.niche_id, n.niche_type "
            "FROM CREATOR_NICHE cn "
            "JOIN NICHE n ON cn.niche_id = n.niche_id "
            "WHERE cn.creator_id = ? AND n.is_deleted = 0 AND n.is_deleted = 0"
        )
    );

    stmt->setInt(1, creatorId);

    auto res = std::unique_ptr<sql::ResultSet>(stmt->executeQuery());

    while (res->next())
    {
        result.emplace_back(res->getInt("niche_id"), res->getString("niche_type"));
    }

    return result;
}



void CreatorNicheRepository::softDelete(int creatorId, int nicheId)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "UPDATE CREATOR_NICHE SET is_deleted = 1 WHERE creator_id = ? AND niche_id = ?"
        )
    );

    stmt->setInt(1, creatorId);
    stmt->setInt(2, nicheId);
    stmt->execute();
}