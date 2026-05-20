#include "Db/Reference/Creator/PlatformRepository.h"

#include <memory>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

PlatformRepository::PlatformRepository(DbManager& db)
    : dbManager(db) {}

int PlatformRepository::insert(const std::string& name)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement("INSERT INTO PLATFORM (platform_name) VALUES (?)")
    );

    stmt->setString(1, name);
    stmt->execute();

    return 0;
}

Platform PlatformRepository::getById(int platformId)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "SELECT platform_id, platform_name FROM PLATFORM WHERE platform_id = ? AND is_deleted = 0"
        )
    );

    stmt->setInt(1, platformId);

    auto res = std::unique_ptr<sql::ResultSet>(stmt->executeQuery());

    if (res->next())
    {
        return Platform(res->getInt("platform_id"), res->getString("platform_name"));
    }

    return Platform();
}

std::vector<Platform> PlatformRepository::getAll()
{
    std::vector<Platform> result;
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement("SELECT platform_id, platform_name FROM PLATFORM WHERE is_deleted = 0")
    );

    auto res = std::unique_ptr<sql::ResultSet>(stmt->executeQuery());

    while (res->next())
    {
        result.emplace_back(res->getInt("platform_id"), res->getString("platform_name"));
    }

    return result;
}

void PlatformRepository::update(int platformId, const std::optional<std::string>& name)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement("UPDATE PLATFORM SET platform_name = ? WHERE platform_id = ?")
    );

    if (name)
        stmt->setString(1, name.value());
    else
        stmt->setNull(1, sql::DataType::VARCHAR);
    stmt->setInt(2, platformId);
    stmt->execute();
}

void PlatformRepository::softDelete(int platformId)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "UPDATE PLATFORM SET is_deleted = 1 WHERE platform_id = ?"
        )
    );

    stmt->setInt(1, platformId);
    stmt->execute();
}