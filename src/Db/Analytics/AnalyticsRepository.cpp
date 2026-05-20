#include "Db/Analytics/AnalyticsRepository.h"

#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

std::vector<std::pair<std::string, int>>
AnalyticsRepository::getMonthlyDeals(sql::Connection* conn)
{
    std::vector<std::pair<std::string, int>> results;

    if (!conn)
        throw std::runtime_error("DB connection is null");

    std::string query =
        "SELECT "
        "DATE_FORMAT(date, '%Y-%m') AS month, "
        "COUNT(*) AS total_deals "
        "FROM DEAL "
        "WHERE is_deleted = 0 "
        "GROUP BY month "
        "ORDER BY month";

    std::unique_ptr<sql::PreparedStatement> pstmt(
        conn->prepareStatement(query)
    );

    std::unique_ptr<sql::ResultSet> res(
        pstmt->executeQuery()
    );

    while (res->next())
    {
        std::string month = res->getString("month");
        int totalDeals = res->getInt("total_deals");

        results.push_back({month, totalDeals});
    }

    return results;
}