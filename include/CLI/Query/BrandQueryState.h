#pragma once

#include <string>
#include <vector>
#include "CLI/UI/TerminalLayout.h"
struct BrandQueryState {
    std::string searchQuery;

    std::vector<int> genreIds;

    std::string sortBy = "name";
    std::string sortOrder = "ASC";
};