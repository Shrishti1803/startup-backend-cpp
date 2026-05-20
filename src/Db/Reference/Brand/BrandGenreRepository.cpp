#include "Db/Reference/Brand/BrandGenreRepository.h"

#include <memory>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

BrandGenreRepository::BrandGenreRepository(DbManager& db)
    : dbManager(db) {}

void BrandGenreRepository::insert(int brandId, int genreId)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "INSERT INTO BRAND_GENRE (brand_id, genre_id, is_deleted) VALUES (?, ?, 0) ON DUPLICATE KEY UPDATE is_deleted = 0"
        )
    );

    stmt->setInt(1, brandId);
    stmt->setInt(2, genreId);
    stmt->execute();
}

std::vector<Genre> BrandGenreRepository::getByBrandId(int brandId)
{
    std::vector<Genre> result;
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "SELECT g.genre_id, g.category "
            "FROM BRAND_GENRE bg "
            "JOIN GENRE g ON bg.genre_id = g.genre_id "
            "WHERE bg.brand_id = ? AND bg.is_deleted = 0 AND g.is_deleted = 0"
        )
    );

    stmt->setInt(1, brandId);

    auto res = std::unique_ptr<sql::ResultSet>(stmt->executeQuery());

    while (res->next())
    {
        result.emplace_back(
            res->getInt("genre_id"),
            res->getString("category")
        );
    }

    return result;
}


void BrandGenreRepository::softDelete(int brandId, int genreId)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "UPDATE BRAND_GENRE SET is_deleted = 1 WHERE brand_id = ? AND genre_id = ?"
        )
    );

    stmt->setInt(1, brandId);
    stmt->setInt(2, genreId);
    stmt->execute();
}

