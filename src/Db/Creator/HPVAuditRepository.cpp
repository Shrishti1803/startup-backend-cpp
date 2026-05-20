#include "Db/Creator/HPVAuditRepository.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

HPVAuditRepository::HPVAuditRepository(DbManager& db)
    : dbManager(db) {}

int HPVAuditRepository::insert(
    sql::Connection* conn,
    int creatorId,
    const std::optional<std::string>& topic,
    const std::optional<std::string>& spikeRate)
{

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "INSERT INTO HPV_AUDIT (creator_id, topic, spike_rate) "
            "VALUES (?, ?, ?)"
        )
    );

    pstmt->setInt(1, creatorId);

    if (topic)
        pstmt->setString(2, topic.value());
    else
        pstmt->setNull(2, sql::DataType::VARCHAR);

    if (spikeRate)
        pstmt->setString(3, spikeRate.value());
    else
        pstmt->setNull(3, sql::DataType::VARCHAR);
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

std::vector<HPVAudit>
HPVAuditRepository::getByCreatorId(int creatorId)
{
    auto conn = dbManager.getConnection();

    std::vector<HPVAudit> audits;

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "SELECT hpv_id, topic, spike_rate "
            "FROM HPV_AUDIT WHERE creator_id=? AND is_deleted=0"
        )
    );

    pstmt->setInt(1, creatorId);

    std::unique_ptr<sql::ResultSet> rs(pstmt->executeQuery());

    while (rs->next()) {

        std::optional<std::string> topic;
        std::optional<std::string> spike;

        if (!rs->isNull("topic"))
            topic = rs->getString("topic");

        if (!rs->isNull("spike_rate"))
            spike = rs->getString("spike_rate");

        audits.emplace_back(
            rs->getInt("hpv_id"),
            topic,
            spike
        );
    }

    return audits;
}

void HPVAuditRepository::update(
    sql::Connection* conn,
    int hpvId,
    const std::optional<std::string>& topic,
    const std::optional<std::string>& spikeRate)
{

    std::string query = "UPDATE HPV_AUDIT SET ";
    std::vector<std::string> fields;

    if (topic) fields.push_back("topic=?");
    if (spikeRate) fields.push_back("spike_rate=?");

    for (size_t i = 0; i < fields.size(); i++) {
        query += fields[i];
        if (i < fields.size() - 1)
            query += ", ";
    }

    query += " WHERE hpv_id=?";

    std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(query));

    int index = 1;

    if (topic) pstmt->setString(index++, topic.value());
    if (spikeRate) pstmt->setString(index++, spikeRate.value());

    pstmt->setInt(index, hpvId);

    pstmt->executeUpdate();
}

void HPVAuditRepository::softDelete(
    sql::Connection* conn,
    int hpvId)
{

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(
            "UPDATE HPV_AUDIT SET is_deleted=1 WHERE hpv_id=?"
        )
    );

    pstmt->setInt(1, hpvId);

    pstmt->executeUpdate();
}

std::optional<HPVAudit>
HPVAuditRepository::getById(
    int hpvId
)
{
    auto conn =
        dbManager.getConnection();

    std::unique_ptr<
        sql::PreparedStatement
    > pstmt(
        conn->prepareStatement(
            "SELECT hpv_id, topic, spike_rate "
            "FROM HPV_AUDIT "
            "WHERE hpv_id=? "
            "AND is_deleted=0"
        )
    );

    pstmt->setInt(1, hpvId);

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
        topic;

    std::optional<std::string>
        spikeRate;

    if (!rs->isNull("topic"))
    {
        topic =
            rs->getString("topic");
    }

    if (!rs->isNull("spike_rate"))
    {
        spikeRate =
            rs->getString(
                "spike_rate"
            );
    }

    return HPVAudit(
        rs->getInt("hpv_id"),
        topic,
        spikeRate
    );
}