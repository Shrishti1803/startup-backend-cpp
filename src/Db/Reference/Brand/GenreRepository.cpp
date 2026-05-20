#include "Db/Reference/Brand/GenreRepository.h"

#include <memory>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

GenreRepository::GenreRepository(DbManager& db)
    : dbManager(db) {}

int GenreRepository::insert(const std::string& category)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "INSERT INTO GENRE (category) VALUES (?)"
        )
    );

    stmt->setString(1, category);

    stmt->execute();
    return 0;
}

Genre GenreRepository::getById(int genreId)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "SELECT genre_id, categoryFROM GENRE WHERE genre_id = ? AND is_deleted = 0"
        )
    );

    stmt->setInt(1, genreId);

    auto res = std::unique_ptr<sql::ResultSet>(stmt->executeQuery());

    if (res->next())
    {
        return Genre(
            res->getInt("genre_id"),
            res->getString("category")
        );
    }

    return Genre();
}

std::vector<Genre> GenreRepository::getAll()
{
    std::vector<Genre> result;
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "SELECT genre_id, category FROM GENRE WHERE is_deleted = 0"
        )
    );

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

void GenreRepository::update(int genreId,
                             const std::optional<std::string>& category)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "UPDATE GENRE SET category = ? WHERE genre_id = ?"
        )
    );

    stmt->setString(1, category.value_or(""));
    stmt->setInt(2, genreId);

    stmt->execute();
}

void GenreRepository::softDelete(int genreId)
{
    auto conn = dbManager.getConnection();

    auto stmt = std::unique_ptr<sql::PreparedStatement>(
        conn->prepareStatement(
            "UPDATE GENRE SET is_deleted = 1 WHERE genre_id = ?"
        )
    );

    stmt->setInt(1, genreId);
    stmt->execute();
}

