#pragma once

#include <vector>
#include <string>
#include "Db/DbManager.h"

namespace sql {
    class Connection;
}
struct BrandResult {
    int brand_id;
    std::string name;
};

class BrandQueryRepository {

private: 
    DbManager& dbManager;
public:
    // Constructor
    BrandQueryRepository(DbManager& db);

    // Search brand names by name / genre / audience
    std::vector<BrandResult> searchBrandNames(
        const std::string& query
    );
    // Filter brands by genre IDs → return ONLY brand names
    std::vector<BrandResult> filterBrandsByGenres(\
        const std::vector<int>& genreIds
    );
    //Sorting & Paginations
    std::vector<BrandResult> getBrands(
        const std::string& searchQuery,
        const std::vector<int>& genreIds,
        const std::string& sortBy,     // "name" / "created_at"
        const std::string& sortOrder,  // "ASC" / "DESC"
        int limit,
        int offset
    );
    //Page limit
    int getBrandCount(const std::string& searchQuery, const std::vector<int>& genreIds);
    
};