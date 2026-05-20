#include "Db/Brand/BrandQueryRepository.h"
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

BrandQueryRepository::BrandQueryRepository(DbManager& db)
    : dbManager(db) {}

//  SEARCH
std::vector<BrandResult> BrandQueryRepository::searchBrandNames(
    const std::string& query
) {
    std::vector<BrandResult> results;
    sql::Connection* conn = dbManager.getConnection();

    if (!conn) {
        throw std::runtime_error("DB connection is null");
    }

    std::string sqlQuery =
        "SELECT DISTINCT b.brand_id, b.brand_name "
        "FROM BRAND b "
        "LEFT JOIN BRAND_GENRE bg ON b.brand_id = bg.brand_id "
        "LEFT JOIN GENRE g ON bg.genre_id = g.genre_id "
        "LEFT JOIN BRAND_TARGET_AUDIENCE ba ON b.brand_id = ba.brand_id "
        "LEFT JOIN TARGET_AUDIENCE ta ON ba.aud_id = ta.aud_id "
        "WHERE (b.brand_name LIKE ? OR category LIKE ? OR ta.aud_name LIKE ?) AND b.is_deleted = 0 ";

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(sqlQuery)
    );

    std::string pattern = "%" + query + "%";

    pstmt->setString(1, pattern);
    pstmt->setString(2, pattern);
    pstmt->setString(3, pattern);

    std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    while (res->next()) {
        BrandResult br;
        br.brand_id = res->getInt("brand_id");
        br.name = res->getString("brand_name");

        results.push_back(br);
    }

    return results;
}


// FILTER (Genre-based)
std::vector<BrandResult> BrandQueryRepository::filterBrandsByGenres(
    const std::vector<int>& genreIds
) {
    std::vector<BrandResult> results;
    sql::Connection* conn = dbManager.getConnection();

    if (!conn) {
        throw std::runtime_error("DB connection is null");
    }

    if (genreIds.empty()) {
        return results;
    }

    // Build (?, ?, ?)
    std::string placeholders;
    for (size_t i = 0; i < genreIds.size(); ++i) {
        placeholders += "?";
        if (i != genreIds.size() - 1) {
            placeholders += ", ";
        }
    }

    std::string query =
        "SELECT DISTINCT b.brand_id, b.brand_name "
        "FROM BRAND b "
        "JOIN BRAND_GENRE bg ON b.brand_id = bg.brand_id "
        "WHERE bg.genre_id IN (" + placeholders + ") AND b.is_deleted = 0";

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(query)
    );

    int index = 1;
    for (int id : genreIds) {
        pstmt->setInt(index++, id);
    }

    std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    while (res->next()) {
        BrandResult br;
        br.brand_id = res->getInt("brand_id");
        br.name = res->getString("brand_name");

        results.push_back(br);
    }

    return results;
}

//SORTING & PAGINATIONS
std::vector<BrandResult> BrandQueryRepository::getBrands(
    const std::string& searchQuery,
    const std::vector<int>& genreIds,
    const std::string& sortBy,
    const std::string& sortOrder,
    int limit,
    int offset
) {
    std::vector<BrandResult> results;
    sql::Connection* conn = dbManager.getConnection();

    if (!conn) {
        throw std::runtime_error("DB connection is null");
    }

    std::string query =
        "SELECT DISTINCT b.brand_id, b.brand_name "
        "FROM BRAND b ";

    // JOIN only if needed
    if (!genreIds.empty()) {
        query += "JOIN BRAND_GENRE bg ON b.brand_id = bg.brand_id ";
    }

    if (!searchQuery.empty()) {
        query +=
            "LEFT JOIN BRAND_GENRE bg2 ON b.brand_id = bg2.brand_id "
            "LEFT JOIN GENRE g ON bg2.genre_id = g.genre_id ";
    }

    query += "WHERE b.is_deleted = 0 ";

    //  Search
    if (!searchQuery.empty()) {
        query += "AND (b.brand_name LIKE ? OR g.category LIKE ?) ";
    }

    // Filter
    if (!genreIds.empty()) {
        std::string placeholders;
        for (size_t i = 0; i < genreIds.size(); ++i) {
            placeholders += "?";
            if (i != genreIds.size() - 1) placeholders += ", ";
        }
        query += "AND bg.genre_id IN (" + placeholders + ") ";
    }

    // Sorting (SAFE)
    std::string orderBy = "b.brand_name"; // default

    if (sortBy == "created_at") {
        orderBy = "b.created_at";
    }

    std::string order = (sortOrder == "DESC") ? "DESC" : "ASC";

    query += "ORDER BY " + orderBy + " " + order + " ";

    // Pagination
    query += "LIMIT ? OFFSET ?";

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(query)
    );

    int index = 1;

    // Bind search
    if (!searchQuery.empty()) {
        std::string pattern = "%" + searchQuery + "%";
        pstmt->setString(index++, pattern);
        pstmt->setString(index++, pattern);
    }

    // Bind genre filters
    for (int id : genreIds) {
        pstmt->setInt(index++, id);
    }

    // Bind pagination
    pstmt->setInt(index++, limit);
    pstmt->setInt(index++, offset);

    std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    while (res->next()) {
        BrandResult br;
        br.brand_id = res->getInt("brand_id");
        br.name = res->getString("brand_name");

        results.push_back(br);
    }

    return results;
}
//Page Limit
int BrandQueryRepository::getBrandCount(
    const std::string& searchQuery,
    const std::vector<int>& genreIds
) {
    sql::Connection* conn = dbManager.getConnection();
    if (!conn) {
        throw std::runtime_error("DB connection is null");
    }

    std::string query =
        "SELECT COUNT(DISTINCT b.brand_id) "
        "FROM BRAND b ";

    // JOIN for filter
    if (!genreIds.empty()) {
        query += "JOIN BRAND_GENRE bg ON b.brand_id = bg.brand_id ";
    }

    // JOIN for search
    if (!searchQuery.empty()) {
        query +=
            "LEFT JOIN BRAND_GENRE bg2 ON b.brand_id = bg2.brand_id "
            "LEFT JOIN GENRE g ON bg2.genre_id = g.genre_id ";
    }

    query += "WHERE b.is_deleted = 0 ";

    // Search
    if (!searchQuery.empty()) {
        query += " AND (b.brand_name LIKE ? OR g.category LIKE ?) ";
    }

    // Filter
    if (!genreIds.empty()) {
        std::string placeholders;
        for (size_t i = 0; i < genreIds.size(); ++i) {
            placeholders += "?";
            if (i != genreIds.size() - 1) placeholders += ", ";
        }
        query += " AND bg.genre_id IN (" + placeholders + ") ";
    }

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(query)
    );

    int index = 1;

    // Bind search
    if (!searchQuery.empty()) {
        std::string pattern = "%" + searchQuery + "%";
        pstmt->setString(index++, pattern);
        pstmt->setString(index++, pattern);
    }

    // Bind genre filters
    for (int id : genreIds) {
        pstmt->setInt(index++, id);
    }

    std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    if (res->next()) {
        return res->getInt(1);
    }

    return 0;
}