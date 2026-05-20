#include "Db/Deal/DealRepository.h"

#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

// 🔹 CREATE
int DealRepository::create(sql::Connection* conn, const Deal& d) {
    if (!conn) throw std::runtime_error("DB connection is null");

    std::string query =
        "INSERT INTO DEAL "
        "(brand_id, creator_id, is_lead, is_connected, is_deal_done, post_uploaded, payment_cleared, date) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?)";

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(query)
    );

    pstmt->setInt(1, d.brand_id);
    pstmt->setInt(2, d.creator_id);
    pstmt->setBoolean(3, d.is_lead);
    pstmt->setBoolean(4, d.is_connected);
    pstmt->setBoolean(5, d.is_deal_done);
    pstmt->setBoolean(6, d.post_uploaded);
    pstmt->setBoolean(7, d.payment_cleared);
    pstmt->setString(8, d.date);

    pstmt->executeUpdate();

    std::unique_ptr<sql::ResultSet> res(
        conn->createStatement()->executeQuery("SELECT LAST_INSERT_ID()")
    );

    if (res->next()) return res->getInt(1);

    return -1;
}

Deal DealRepository::getById(sql::Connection* conn, int deal_id) {
    if (!conn) throw std::runtime_error("DB connection is null");

    std::string query =
        "SELECT * FROM DEAL WHERE deal_id = ? AND is_deleted = 0";

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(query)
    );

    pstmt->setInt(1, deal_id);

    std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    if (!res->next()) {
        throw std::runtime_error("Deal not found");
    }

    Deal d;
    d.deal_id = res->getInt("deal_id");
    d.brand_id = res->getInt("brand_id");
    d.creator_id = res->getInt("creator_id");

    d.is_lead = res->getBoolean("is_lead");
    d.is_connected = res->getBoolean("is_connected");
    d.is_deal_done = res->getBoolean("is_deal_done");
    d.post_uploaded = res->getBoolean("post_uploaded");
    d.payment_cleared = res->getBoolean("payment_cleared");

    d.date = res->getString("date");
    d.is_deleted = res->getBoolean("is_deleted");

    return d;
}

std::vector<Deal> DealRepository::getByBrandId(sql::Connection* conn, int brand_id) {
    std::vector<Deal> results;

    std::string query =
        "SELECT * FROM DEAL WHERE brand_id = ? AND is_deleted = 0";

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(query)
    );

    pstmt->setInt(1, brand_id);

    std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    while (res->next()) {
        Deal d;
        d.deal_id = res->getInt("deal_id");
        d.brand_id = res->getInt("brand_id");
        d.creator_id = res->getInt("creator_id");

        d.is_lead = res->getBoolean("is_lead");
        d.is_connected = res->getBoolean("is_connected");
        d.is_deal_done = res->getBoolean("is_deal_done");
        d.post_uploaded = res->getBoolean("post_uploaded");
        d.payment_cleared = res->getBoolean("payment_cleared");

        d.date = res->getString("date");
        d.is_deleted = res->getBoolean("is_deleted");

        results.push_back(d);
    }

    return results;
}

std::vector<Deal> DealRepository::getByCreatorId(sql::Connection* conn, int creator_id) {
    std::vector<Deal> results;

    std::string query =
        "SELECT * FROM DEAL WHERE creator_id = ? AND is_deleted = 0";

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(query)
    );

    pstmt->setInt(1, creator_id);

    std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    while (res->next()) {
        Deal d;
        d.deal_id = res->getInt("deal_id");
        d.brand_id = res->getInt("brand_id");
        d.creator_id = res->getInt("creator_id");

        d.is_lead = res->getBoolean("is_lead");
        d.is_connected = res->getBoolean("is_connected");
        d.is_deal_done = res->getBoolean("is_deal_done");
        d.post_uploaded = res->getBoolean("post_uploaded");
        d.payment_cleared = res->getBoolean("payment_cleared");

        d.date = res->getString("date");
        d.is_deleted = res->getBoolean("is_deleted");

        results.push_back(d);
    }

    return results;
}

void DealRepository::update(
    sql::Connection* conn,
    int deal_id,
    const DealUpdate& u
) {
    if (!conn) throw std::runtime_error("DB connection is null");

    std::string query = "UPDATE DEAL SET ";
    std::vector<std::string> fields;

    if (u.brand_id.has_value()) fields.push_back("brand_id = ?");
    if (u.creator_id.has_value()) fields.push_back("creator_id = ?");
    if (u.is_lead.has_value()) fields.push_back("is_lead = ?");
    if (u.is_connected.has_value()) fields.push_back("is_connected = ?");
    if (u.is_deal_done.has_value()) fields.push_back("is_deal_done = ?");
    if (u.post_uploaded.has_value()) fields.push_back("post_uploaded = ?");
    if (u.payment_cleared.has_value()) fields.push_back("payment_cleared = ?");
    if (u.date.has_value()) fields.push_back("date = ?");

    if (fields.empty()) return;

    for (size_t i = 0; i < fields.size(); ++i) {
        query += fields[i];
        if (i != fields.size() - 1) query += ", ";
    }

    query += " WHERE deal_id = ? AND is_deleted = 0";

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(query)
    );

    int index = 1;

    if (u.brand_id.has_value()) pstmt->setInt(index++, *u.brand_id);
    if (u.creator_id.has_value()) pstmt->setInt(index++, *u.creator_id);
    if (u.is_lead.has_value()) pstmt->setBoolean(index++, *u.is_lead);
    if (u.is_connected.has_value()) pstmt->setBoolean(index++, *u.is_connected);
    if (u.is_deal_done.has_value()) pstmt->setBoolean(index++, *u.is_deal_done);
    if (u.post_uploaded.has_value()) pstmt->setBoolean(index++, *u.post_uploaded);
    if (u.payment_cleared.has_value()) pstmt->setBoolean(index++, *u.payment_cleared);
    if (u.date.has_value()) pstmt->setString(index++, *u.date);

    pstmt->setInt(index++, deal_id);

    pstmt->executeUpdate();
}

void DealRepository::softDelete(sql::Connection* conn, int deal_id) {
    if (!conn) throw std::runtime_error("DB connection is null");

    std::string query =
        "UPDATE DEAL SET is_deleted = 1 WHERE deal_id = ?";

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(query)
    );

    pstmt->setInt(1, deal_id);
    pstmt->executeUpdate();
}

std::vector<DealView> DealRepository::getDealsWithNames(sql::Connection* conn) {
    std::vector<DealView> results;

    if (!conn) throw std::runtime_error("DB connection is null");

    std::string query =
        "SELECT d.deal_id, b.brand_name AS brand_name, c.creator_name AS creator_name, "
        "d.is_lead, d.is_connected, d.is_deal_done, d.post_uploaded, d.payment_cleared, d.date "
        "FROM DEAL d "
        "LEFT JOIN BRAND b ON d.brand_id = b.brand_id "
        "LEFT JOIN CREATOR c ON d.creator_id = c.creator_id "
        "WHERE d.is_deleted = 0";

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(query)
    );

    std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    while (res->next()) {
        DealView dv;

        dv.deal_id = res->getInt("deal_id");
        dv.brand_name = res->getString("brand_name");
        dv.creator_name = res->getString("creator_name");

        dv.is_lead = res->getBoolean("is_lead");
        dv.is_connected = res->getBoolean("is_connected");
        dv.is_deal_done = res->getBoolean("is_deal_done");
        dv.post_uploaded = res->getBoolean("post_uploaded");
        dv.payment_cleared = res->getBoolean("payment_cleared");

        dv.date = res->getString("date");

        results.push_back(dv);
    }

    return results;
}

bool DealRepository::existsById(sql::Connection* conn, int dealId)
{
    std::unique_ptr<sql::PreparedStatement> stmt(
        conn->prepareStatement(
            "SELECT 1 FROM DEAL WHERE deal_id = ? AND is_deleted = 0 LIMIT 1"
        )
    );

    stmt->setInt(1, dealId);

    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

    return res->next();
}
