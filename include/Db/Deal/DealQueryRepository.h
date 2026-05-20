#pragma once

#include <vector>
#include <string>
#include "Models/Deals/DealView.h"

namespace sql {
    class Connection;
}

class DealQueryRepository {
public:
    std::vector<DealView> filterDeals(
        sql::Connection* conn,
        const std::string& brandName,
        const std::string& creatorName
    );
    std::vector<DealView> getDeals(
        sql::Connection* conn,
        const std::string& brandName,
        const std::string& creatorName,
        const std::string& sortBy,      // "date" / "brand_name" / "creator_name"
        const std::string& sortOrder,   // "ASC" / "DESC"
        int limit,
        int offset
    );

    int getDealCount(
        sql::Connection* conn,
        const std::string& brandName,
        const std::string& creatorName
    );
};