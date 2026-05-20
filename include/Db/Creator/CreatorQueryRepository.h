#pragma once

#include <vector>
#include <string>

namespace sql {
    class Connection;
}

struct CreatorResult {
    int creator_id;
    std::string name;
};

class CreatorQueryRepository {
public:
    std::vector<CreatorResult> searchCreators(
        sql::Connection* conn,
        const std::string& query
    );
    std::vector<CreatorResult> filterCreators(
        sql::Connection* conn,
        const std::vector<int>& nicheIds,
        int minAudienceSize,
        int maxAudienceSize
    );
    std::vector<CreatorResult> getCreators(
        sql::Connection* conn,
        const std::string& searchQuery,
        const std::vector<int>& nicheIds,
        int minAudienceSize,
        int maxAudienceSize,
        const std::string& sortBy,      // "name" / "audience_size"
        const std::string& sortOrder,   // "ASC" / "DESC"
        int limit,
        int offset
    );

    int getCreatorCount(
        sql::Connection* conn,
        const std::string& searchQuery,
        const std::vector<int>& nicheIds,
        int minAudienceSize,
        int maxAudienceSize
    );
};