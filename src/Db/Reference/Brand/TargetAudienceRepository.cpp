#include "Db/Reference/Brand/TargetAudienceRepository.h"

#include <memory>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

TargetAudienceRepository::TargetAudienceRepository(DbManager& db)
    : dbManager(db) {}

int TargetAudienceRepository::insert(const std::string& name)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "INSERT INTO TARGET_AUDIENCE (aud_name) VALUES (?)"
        )
    );

    stmt->setString(1, name);
    stmt->execute();

    return 0;
}

TargetAudience TargetAudienceRepository::getById(int audId)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "SELECT aud_id, aud_name FROM TARGET_AUDIENCE WHERE aud_id = ? AND is_deleted = 0"
        )
    );

    stmt->setInt(1, audId);

    auto res = std::unique_ptr<sql::ResultSet>(stmt->executeQuery());

    if (res->next())
    {
        return TargetAudience(
            res->getInt("aud_id"),
            res->getString("aud_name")
        );
    }

    return TargetAudience();
}

std::vector<TargetAudience> TargetAudienceRepository::getAll()
{
    std::vector<TargetAudience> result;
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "SELECT aud_id, aud_name FROM TARGET_AUDIENCE WHERE is_deleted = 0"
        )
    );

    auto res = std::unique_ptr<sql::ResultSet>(stmt->executeQuery());

    while (res->next())
    {
        result.emplace_back(
            res->getInt("aud_id"),
            res->getString("aud_name")
        );
    }

    return result;
}

void TargetAudienceRepository::update(int audId,
                                      const std::optional<std::string>& name)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "UPDATE TARGET_AUDIENCE SET aud_name = ? WHERE aud_id = ?"
        )
    );

    stmt->setString(1, name.value_or(""));
    stmt->setInt(2, audId);

    stmt->execute();
}

void TargetAudienceRepository::softDelete(int audId)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "UPDATE TARGET_AUDIENCE SET is_deleted = 1 WHERE aud_id = ?"
        )
    );

    stmt->setInt(1, audId);
    stmt->execute();
}