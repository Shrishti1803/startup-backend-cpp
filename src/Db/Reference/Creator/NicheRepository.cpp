#include "Db/Reference/Creator/NicheRepository.h"

#include <memory>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

NicheRepository::NicheRepository(DbManager& db)
    : dbManager(db) {}

int NicheRepository::insert(const std::string& nicheType)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement("INSERT INTO NICHE (niche_type) VALUES (?)")
    );

    stmt->setString(1, nicheType);
    stmt->execute();

    return 0;
}

Niche NicheRepository::getById(int nicheId)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "SELECT niche_id, niche_type FROM NICHE WHERE niche_id = ? AND is_deleted = 0"
        )
    );

    stmt->setInt(1, nicheId);

    auto res = std::unique_ptr<sql::ResultSet>(stmt->executeQuery());

    if (res->next())
    {
        return Niche(res->getInt("niche_id"), res->getString("niche_type"));
    }

    return Niche();
}

std::vector<Niche> NicheRepository::getAll()
{
    std::vector<Niche> result;
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement("SELECT niche_id, niche_type FROM NICHE WHERE is_deleted = 0")
    );

    auto res = std::unique_ptr<sql::ResultSet>(stmt->executeQuery());

    while (res->next())
    {
        result.emplace_back(res->getInt("niche_id"), res->getString("niche_type"));
    }

    return result;
}

void NicheRepository::update(int nicheId, const std::optional<std::string>& nicheType)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement("UPDATE NICHE SET niche_type = ? WHERE niche_id = ?")
    );

    if (nicheType)
        stmt->setString(1, nicheType.value());
    else
        stmt->setNull(1, sql::DataType::VARCHAR);
    stmt->setInt(2, nicheId);
    stmt->execute();
}

void NicheRepository::softDelete(int nicheId)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "UPDATE NICHE SET is_deleted = 1 WHERE niche_id = ?"
        )
    );

    stmt->setInt(1, nicheId);
    stmt->execute();
}