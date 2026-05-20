#include "Db/Reference/Brand/BrandTargetAudienceRepository.h"

#include <memory>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

BrandTargetAudienceRepository::BrandTargetAudienceRepository(DbManager& db)
    : dbManager(db) {}

void BrandTargetAudienceRepository::insert(int brandId, int audId)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "INSERT INTO BRAND_TARGET_AUDIENCE (brand_id, aud_id, is_deleted) VALUES (?, ?, 0) ON DUPLICATE KEY UPDATE is_deleted = 0"
        )
    );

    stmt->setInt(1, brandId);
    stmt->setInt(2, audId);
    stmt->execute();
}

std::vector<TargetAudience> BrandTargetAudienceRepository::getByBrandId(int brandId)
{
    std::vector<TargetAudience> result;
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "SELECT ta.aud_id, ta.aud_name "
            "FROM BRAND_TARGET_AUDIENCE bta "
            "JOIN TARGET_AUDIENCE ta ON bta.aud_id = ta.aud_id "
            "WHERE bta.brand_id = ? AND bta.is_deleted = 0 AND ta.is_deleted = 0"
        )
    );

    stmt->setInt(1, brandId);

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


void BrandTargetAudienceRepository::softDelete(int brandId, int audId)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "UPDATE BRAND_TARGET_AUDIENCE SET is_deleted = 1 WHERE brand_id = ? AND aud_id = ?"
        )
    );

    stmt->setInt(1, brandId);
    stmt->setInt(2, audId);
    stmt->execute();
}