#include "Db/Creator/StrategicNoteRepository.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

StrategicNoteRepository::StrategicNoteRepository(DbManager& db)
    : dbManager(db) {}

int StrategicNoteRepository::insert(
    sql::Connection* conn,
    int creatorId,
    const std::optional<std::string>& description,
    const std::string& author)
{
    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "INSERT INTO STRATEGIC_NOTE (creator_id, description, author_name) "
            "VALUES (?, ?, ?)"
        )
    );

    pstmt->setInt(1, creatorId);

    if (description)
        pstmt->setString(2, description.value());
    else
        pstmt->setNull(2, sql::DataType::VARCHAR);

    pstmt->setString(3, author);

    pstmt->executeUpdate();

    std::unique_ptr<sql::Statement> stmt(
        conn->createStatement()
    );

    std::unique_ptr<sql::ResultSet> rs(
        stmt->executeQuery(
            "SELECT LAST_INSERT_ID()"
        )
    );

    if(rs->next())
    {
        return rs->getInt(1);
    }

    return -1;
}

std::vector<StrategicNote>
StrategicNoteRepository::getByCreatorId(int creatorId)
{
    auto conn = dbManager.getConnection();

    std::vector<StrategicNote> notes;

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "SELECT strategic_id, description, author_name "
            "FROM STRATEGIC_NOTE WHERE creator_id=? AND is_deleted=0"
        )
    );

    pstmt->setInt(1, creatorId);

    std::unique_ptr<sql::ResultSet> rs(pstmt->executeQuery());

    while (rs->next()) {

        std::optional<std::string> description;

        if (!rs->isNull("description"))
            description = rs->getString("description");

        notes.emplace_back(
            rs->getInt("strategic_id"),
            rs->getString("author_name"),
            description
        );
    }

    return notes;
}

void StrategicNoteRepository::update(
    sql::Connection* conn,
    int strategicId,
    const std::optional<std::string>& description,
    const std::optional<std::string>& author)
{

    std::string query = "UPDATE STRATEGIC_NOTE SET ";
    std::vector<std::string> fields;

    if (description) fields.push_back("description=?");
    if (author) fields.push_back("author_name=?");

    for (size_t i = 0; i < fields.size(); i++) {
        query += fields[i];
        if (i < fields.size() - 1)
            query += ", ";
    }

    query += " WHERE strategic_id=?";

    std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(query));

    int index = 1;

    if (description) pstmt->setString(index++, description.value());
    if (author) pstmt->setString(index++, author.value());

    pstmt->setInt(index, strategicId);

    pstmt->executeUpdate();
}

void StrategicNoteRepository::softDelete(
    sql::Connection* conn,
    int strategicId)
{
    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "UPDATE STRATEGIC_NOTE SET is_deleted=1 WHERE strategic_id=?"
        )
    );

    pstmt->setInt(1, strategicId);

    pstmt->executeUpdate();
}

std::optional<StrategicNote>
StrategicNoteRepository::getById(
    int strategicId
)
{
    auto conn =
        dbManager.getConnection();

    std::unique_ptr<
        sql::PreparedStatement
    > pstmt(
        conn->prepareStatement(
            "SELECT strategic_id, "
            "description, author_name "
            "FROM STRATEGIC_NOTE "
            "WHERE strategic_id=? "
            "AND is_deleted=0"
        )
    );

    pstmt->setInt(
        1,
        strategicId
    );

    std::unique_ptr<
        sql::ResultSet
    > rs(
        pstmt->executeQuery()
    );

    if (!rs->next())
    {
        return std::nullopt;
    }

    std::optional<std::string>
        desc;

    if (
        !rs->isNull(
            "description"
        )
    )
    {
        desc =
            rs->getString(
                "description"
            );
    }

    return StrategicNote(
        rs->getInt(
            "strategic_id"
        ),
        rs->getString(
            "author_name"
        ),
        desc
    );
}