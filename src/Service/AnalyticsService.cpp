#include "Service/AnalyticsService.h"

#include <fstream>
#include <cstdlib>

AnalyticsService::AnalyticsService(DbManager& db)
    : db(db)
{}

void AnalyticsService::showMonthlyDealsGraph()
{
    auto conn = db.getConnection();

    auto data =
        analyticsRepository.getMonthlyDeals(conn);

    std::ofstream file("analytics.csv");

    file << "month,total_deals\n";

    for (const auto& [month, count] : data)
    {
        file
            << month
            << ","
            << count
            << "\n";
    }

    file.close();

    system("python3 include/Db/Analytics/Analytics.py");
}