#include "Db/Creator/VerdictRepository.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

VerdictRepository::VerdictRepository(DbManager& db)
    : dbManager(db) {}

int VerdictRepository::insert(
    sql::Connection* conn,
    int creatorId,
    const std::optional<std::string>& strengths,
    const std::optional<std::string>& weaknesses,
    const std::optional<std::string>& growth,
    const std::optional<std::string>& finalVerdict)
{
    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "INSERT INTO VERDICT "
            "(creator_id, strengths, weaknesses, growth_potential, final_verdict) "
            "VALUES (?, ?, ?, ?, ?)"
        )
    );

    pstmt->setInt(1, creatorId);

    if (strengths) pstmt->setString(2, strengths.value());
    else pstmt->setNull(2, sql::DataType::VARCHAR);

    if (weaknesses) pstmt->setString(3, weaknesses.value());
    else pstmt->setNull(3, sql::DataType::VARCHAR);

    if (growth) pstmt->setString(4, growth.value());
    else pstmt->setNull(4, sql::DataType::VARCHAR);

    if (finalVerdict) pstmt->setString(5, finalVerdict.value());
    else pstmt->setNull(5, sql::DataType::VARCHAR);

    pstmt->executeUpdate();

    return 1;
}

std::optional<Verdict>
VerdictRepository::getByCreatorId(int creatorId)
{
    auto conn = dbManager.getConnection();

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "SELECT verdict_id, strengths, weaknesses, growth_potential, final_verdict "
            "FROM VERDICT WHERE creator_id=? AND is_deleted=0"
        )
    );

    pstmt->setInt(1, creatorId);

    std::unique_ptr<sql::ResultSet> rs(pstmt->executeQuery());

    if (!rs->next())
        return std::nullopt;

    return Verdict(
        rs->getInt("verdict_id"),
        rs->getString("strengths"),
        rs->getString("weaknesses"),
        rs->getString("growth_potential"),
        rs->getString("final_verdict")
    );
}

void VerdictRepository::update(
    sql::Connection* conn,
    int verdictId,
    const std::optional<std::string>& strengths,
    const std::optional<std::string>& weaknesses,
    const std::optional<std::string>& growth,
    const std::optional<std::string>& finalVerdict)
{

    std::string query = "UPDATE VERDICT SET ";
    std::vector<std::string> fields;

    if (strengths) fields.push_back("strengths=?");
    if (weaknesses) fields.push_back("weaknesses=?");
    if (growth) fields.push_back("growth_potential=?");
    if (finalVerdict) fields.push_back("final_verdict=?");

    for (size_t i = 0; i < fields.size(); i++) {
        query += fields[i];
        if (i < fields.size() - 1)
            query += ", ";
    }

    query += " WHERE verdict_id=?";

    std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(query));

    int index = 1;

    if (strengths) pstmt->setString(index++, strengths.value());
    if (weaknesses) pstmt->setString(index++, weaknesses.value());
    if (growth) pstmt->setString(index++, growth.value());
    if (finalVerdict) pstmt->setString(index++, finalVerdict.value());

    pstmt->setInt(index, verdictId);

    pstmt->executeUpdate();
}

void VerdictRepository::softDelete(
    sql::Connection* conn,
    int verdictId)
{

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "UPDATE VERDICT SET is_deleted=1 WHERE verdict_id=?"
        )
    );

    pstmt->setInt(1, verdictId);

    pstmt->executeUpdate();
}