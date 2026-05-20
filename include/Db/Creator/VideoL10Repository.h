#pragma once

#include <vector>
#include <optional>

#include "Models/Creator/VideoL10.h"
#include "Db/DbManager.h"
#include <cppconn/connection.h>

class VideoL10Repository {

private:
    DbManager& dbManager;

public:

    VideoL10Repository(DbManager& db);

    int insert(
        sql::Connection* conn,
        int creatorId,
        const std::optional<std::string>& consistency,
        const std::optional<std::string>& avgViews,
        const std::optional<std::string>& avgER,
        const std::optional<std::string>& sharability,
        const std::optional<std::string>& velocityTrend
    );

    std::vector<VideoL10> getByCreatorId(int creatorId);

    void update(
        sql::Connection* conn,
        int performanceId,
        const std::optional<std::string>& consistency,
        const std::optional<std::string>& avgViews,
        const std::optional<std::string>& avgER,
        const std::optional<std::string>& sharability,
        const std::optional<std::string>& velocityTrend
    );

    void softDelete(sql::Connection* conn,
                    int performanceId);

    std::optional<VideoL10> getById(
        int performanceId
    );
};