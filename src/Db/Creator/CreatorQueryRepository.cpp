#include "Db/Creator/CreatorQueryRepository.h"

#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

//Searching
std::vector<CreatorResult> CreatorQueryRepository::searchCreators(
    sql::Connection* conn,
    const std::string& query
) {
    std::vector<CreatorResult> results;

    if (!conn) {
        throw std::runtime_error("DB connection is null");
    }

    std::string sqlQuery =
        "SELECT DISTINCT c.creator_id, c.creator_name "
        "FROM CREATOR c "
        "LEFT JOIN CREATOR_NICHE cn ON c.creator_id = cn.creator_id "
        "LEFT JOIN NICHE n ON cn.niche_id = n.niche_id "
        "LEFT JOIN CREATOR_TARGET_AUDIENCE cta ON c.creator_id = cta.creator_id "
        "LEFT JOIN TARGET_AUDIENCE ta ON cta.aud_id = ta.aud_id "
        "WHERE (c.creator_name LIKE ? OR n.niche_type LIKE ? OR ta.aud_name LIKE ?) AND c.is_deleted = 0";

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(sqlQuery)
    );

    std::string pattern = "%" + query + "%";

    pstmt->setString(1, pattern);
    pstmt->setString(2, pattern);
    pstmt->setString(3, pattern);

    std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    while (res->next()) {
        CreatorResult cr;
        cr.creator_id = res->getInt("creator_id");
        cr.name = res->getString("creator_name");

        results.push_back(cr);
    }

    return results;
}

//Filtering
std::vector<CreatorResult> CreatorQueryRepository::filterCreators(
    sql::Connection* conn,
    const std::vector<int>& nicheIds,
    int minAudienceSize,
    int maxAudienceSize
) {
    std::vector<CreatorResult> results;

    if (!conn) {
        throw std::runtime_error("DB connection is null");
    }

    std::string query =
        "SELECT DISTINCT c.creator_id, c.creator_name "
        "FROM CREATOR c ";

    
    if (!nicheIds.empty()) {
        query += "JOIN CREATOR_NICHE cn ON c.creator_id = cn.creator_id ";
    }
    

    query += "WHERE c.is_deleted = 0 ";

    
    if (!nicheIds.empty()) {
        std::string placeholders;
        for (size_t i = 0; i < nicheIds.size(); ++i) {
            placeholders += "?";
            if (i != nicheIds.size() - 1) placeholders += ", ";
        }
        query += "AND cn.niche_id IN (" + placeholders + ") ";
    }
    

    // Audience size filter
    if (minAudienceSize >= 0 && maxAudienceSize >= 0) {
        query += "AND c.audience_size BETWEEN ? AND ? ";
    }

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(query)
    );

    int index = 1;


    for (int id : nicheIds) {
        pstmt->setInt(index++, id);
    }
    

    if (minAudienceSize >= 0 && maxAudienceSize >= 0) {
        pstmt->setInt(index++, minAudienceSize);
        pstmt->setInt(index++, maxAudienceSize);
    }

    std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    while (res->next()) {
        CreatorResult cr;
        cr.creator_id = res->getInt("creator_id");
        cr.name = res->getString("creator_name");

        results.push_back(cr);
    }

    return results;
}

//SORTING & PAGINATIONS
std::vector<CreatorResult> CreatorQueryRepository::getCreators(
    sql::Connection* conn,
    const std::string& searchQuery,
    const std::vector<int>& nicheIds,
    int minAudienceSize,
    int maxAudienceSize,
    const std::string& sortBy,
    const std::string& sortOrder,
    int limit,
    int offset
) {
    std::vector<CreatorResult> results;

    if (!conn) {
        throw std::runtime_error("DB connection is null");
    }

    std::string query =
        "SELECT DISTINCT c.creator_id, c.creator_name "
        "FROM CREATOR c ";


    if (!nicheIds.empty()) {
        query += "JOIN CREATOR_NICHE cn ON c.creator_id = cn.creator_id ";
    }
    

    // KEEP for search
    if (!searchQuery.empty()) {
        query +=
            "LEFT JOIN CREATOR_NICHE cn2 ON c.creator_id = cn2.creator_id "
            "LEFT JOIN NICHE n ON cn2.niche_id = n.niche_id "
            "LEFT JOIN CREATOR_TARGET_AUDIENCE cta ON c.creator_id = cta.creator_id "
            "LEFT JOIN TARGET_AUDIENCE ta ON cta.aud_id = ta.aud_id ";
    }

    query += "WHERE c.is_deleted = 0 ";

    if (!searchQuery.empty()) {
        query += "AND (c.creator_name LIKE ? OR n.niche_type LIKE ? OR ta.aud_name LIKE ?) ";
    }


    if (!nicheIds.empty()) {
        std::string placeholders;
        for (size_t i = 0; i < nicheIds.size(); ++i) {
            placeholders += "?";
            if (i != nicheIds.size() - 1) placeholders += ", ";
        }
        query += "AND cn.niche_id IN (" + placeholders + ") ";
    }
    

    if (minAudienceSize >= 0 && maxAudienceSize >= 0) {
        query += "AND c.audience_size BETWEEN ? AND ? ";
    }

    std::string orderBy = "c.creator_name";
    if (sortBy == "audience_size") {
        orderBy = "c.audience_size";
    }

    std::string order = (sortOrder == "DESC") ? "DESC" : "ASC";
    query += "ORDER BY " + orderBy + " " + order + " ";

    query += "LIMIT ? OFFSET ?";

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(query)
    );

    int index = 1;

    if (!searchQuery.empty()) {
        std::string pattern = "%" + searchQuery + "%";
        pstmt->setString(index++, pattern);
        pstmt->setString(index++, pattern);
        pstmt->setString(index++, pattern);
    }


    for (int id : nicheIds) {
        pstmt->setInt(index++, id);
    }
    

    if (minAudienceSize >= 0 && maxAudienceSize >= 0) {
        pstmt->setInt(index++, minAudienceSize);
        pstmt->setInt(index++, maxAudienceSize);
    }

    pstmt->setInt(index++, limit);
    pstmt->setInt(index++, offset);

    std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    while (res->next()) {
        CreatorResult cr;
        cr.creator_id = res->getInt("creator_id");
        cr.name = res->getString("creator_name");

        results.push_back(cr);
    }

    return results;
}

//Creator count
int CreatorQueryRepository::getCreatorCount(
    sql::Connection* conn,
    const std::string& searchQuery,
    const std::vector<int>& nicheIds,
    int minAudienceSize,
    int maxAudienceSize
) {
    if (!conn) {
        throw std::runtime_error("DB connection is null");
    }

    std::string query =
        "SELECT COUNT(DISTINCT c.creator_id) "
        "FROM CREATOR c ";


    if (!nicheIds.empty()) {
        query += "JOIN CREATOR_NICHE cn ON c.creator_id = cn.creator_id ";
    }
    

    if (!searchQuery.empty()) {
        query +=
            "LEFT JOIN CREATOR_NICHE cn2 ON c.creator_id = cn2.creator_id "
            "LEFT JOIN NICHE n ON cn2.niche_id = n.niche_id "
            "LEFT JOIN CREATOR_TARGET_AUDIENCE cta ON c.creator_id = cta.creator_id "
            "LEFT JOIN TARGET_AUDIENCE ta ON cta.aud_id = ta.aud_id ";
    }

    query += "WHERE c.is_deleted = 0 ";

    if (!searchQuery.empty()) {
        query += "AND (c.creator_name LIKE ? OR n.niche_type LIKE ? OR ta.aud_name LIKE ?) ";
    }


    if (!nicheIds.empty()) {
        std::string placeholders;
        for (size_t i = 0; i < nicheIds.size(); ++i) {
            placeholders += "?";
            if (i != nicheIds.size() - 1) placeholders += ", ";
        }
        query += "AND cn.niche_id IN (" + placeholders + ") ";
    }
    

    if (minAudienceSize >= 0 && maxAudienceSize >= 0) {
        query += "AND c.audience_size BETWEEN ? AND ? ";
    }

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(query)
    );

    int index = 1;

    if (!searchQuery.empty()) {
        std::string pattern = "%" + searchQuery + "%";
        pstmt->setString(index++, pattern);
        pstmt->setString(index++, pattern);
        pstmt->setString(index++, pattern);
    }


    for (int id : nicheIds) {
        pstmt->setInt(index++, id);
    }
    

    if (minAudienceSize >= 0 && maxAudienceSize >= 0) {
        pstmt->setInt(index++, minAudienceSize);
        pstmt->setInt(index++, maxAudienceSize);
    }

    std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    if (res->next()) {
        return res->getInt(1);
    }

    return 0;
}