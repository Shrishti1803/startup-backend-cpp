#pragma once

#include <optional>

#include "Models/Creator/CommunityReaches.h"
#include "Db/DbManager.h"
#include <cppconn/connection.h>

class CommunityReachesRepository {

private:
    DbManager& dbManager;

public:

    CommunityReachesRepository(DbManager& db);

    int insert(
        sql::Connection* conn,
        int creatorId,
        const std::optional<std::string>& sentiment,
        const std::optional<std::string>& replyRate
    );

    std::optional<CommunityReaches> getByCreatorId(int creatorId);

    void update(
        sql::Connection* conn,
        int commId,
        const std::optional<std::string>& sentiment,
        const std::optional<std::string>& replyRate
    );

    void softDelete(sql::Connection* conn,
                    int commId);
};