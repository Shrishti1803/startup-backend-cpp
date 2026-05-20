#pragma once

#include <vector>
#include <utility>
#include <string>

namespace sql
{
    class Connection;
}

class AnalyticsRepository
{
public:
    std::vector<std::pair<std::string, int>>
    getMonthlyDeals(sql::Connection* conn);
};