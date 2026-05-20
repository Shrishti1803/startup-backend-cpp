#include "Db/Deal/DealQueryRepository.h"

#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

std::vector<DealView> DealQueryRepository::filterDeals(
    sql::Connection* conn,
    const std::string& brandName,
    const std::string& creatorName
) {
    std::vector<DealView> results;

    if (!conn) {
        throw std::runtime_error("DB connection is null");
    }

    std::string query =
        "SELECT d.deal_id, b.brand_name AS brand_name, c.creator_name AS creator_name, "
        "d.is_lead, d.is_connected, d.is_deal_done, d.post_uploaded, d.payment_cleared, d.date "
        "FROM DEAL d "
        "LEFT JOIN BRAND b ON d.brand_id = b.brand_id "
        "LEFT JOIN CREATOR c ON d.creator_id = c.creator_id "
        "WHERE d.is_deleted = 0 ";

    // Filters
    if (!brandName.empty()) {
        query += "AND b.brand_name LIKE ? ";
    }

    if (!creatorName.empty()) {
        query += "AND c.creator_name LIKE ? ";
    }

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(query)
    );

    int index = 1;

    if (!brandName.empty()) {
        pstmt->setString(index++, "%" + brandName + "%");
    }

    if (!creatorName.empty()) {
        pstmt->setString(index++, "%" + creatorName + "%");
    }

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

std::vector<DealView> DealQueryRepository::getDeals(
    sql::Connection* conn,
    const std::string& brandName,
    const std::string& creatorName,
    const std::string& sortBy,
    const std::string& sortOrder,
    int limit,
    int offset
) {
    std::vector<DealView> results;

    if (!conn) throw std::runtime_error("DB connection is null");

    std::string query =
        "SELECT d.deal_id, b.brand_name AS brand_name, c.creator_name AS creator_name, "
        "d.is_lead, d.is_connected, d.is_deal_done, d.post_uploaded, d.payment_cleared, d.date "
        "FROM DEAL d "
        "LEFT JOIN BRAND b ON d.brand_id = b.brand_id "
        "LEFT JOIN CREATOR c ON d.creator_id = c.creator_id "
        "WHERE d.is_deleted = 0 ";

    // Filters
    if (!brandName.empty()) query += "AND b.brand_name LIKE ? ";
    if (!creatorName.empty()) query += "AND c.creator_name LIKE ? ";

    // Sorting (SAFE)
    std::string orderBy = "d.date";

    if (sortBy == "brand_name") orderBy = "b.brand_name";
    else if (sortBy == "creator_name") orderBy = "c.creator_name";

    std::string order = (sortOrder == "DESC") ? "DESC" : "ASC";

    query += "ORDER BY " + orderBy + " " + order + " ";

    // Pagination
    query += "LIMIT ? OFFSET ?";

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(query)
    );

    int index = 1;

    if (!brandName.empty())
        pstmt->setString(index++, "%" + brandName + "%");

    if (!creatorName.empty())
        pstmt->setString(index++, "%" + creatorName + "%");

    pstmt->setInt(index++, limit);
    pstmt->setInt(index++, offset);

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

int DealQueryRepository::getDealCount(
    sql::Connection* conn,
    const std::string& brandName,
    const std::string& creatorName
) {
    if (!conn) throw std::runtime_error("DB connection is null");

    std::string query =
        "SELECT COUNT(*) "
        "FROM DEAL d "
        "LEFT JOIN BRAND b ON d.brand_id = b.brand_id "
        "LEFT JOIN CREATOR c ON d.creator_id = c.creator_id "
        "WHERE d.is_deleted = 0 ";

    if (!brandName.empty()) query += "AND b.brand_name LIKE ? ";
    if (!creatorName.empty()) query += "AND c.creator_name LIKE ? ";

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(query)
    );

    int index = 1;

    if (!brandName.empty())
        pstmt->setString(index++, "%" + brandName + "%");

    if (!creatorName.empty())
        pstmt->setString(index++, "%" + creatorName + "%");

    std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    if (res->next()) return res->getInt(1);

    return 0;
}