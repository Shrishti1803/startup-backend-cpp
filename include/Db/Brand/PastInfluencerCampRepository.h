#pragma once

#include "Db/DbManager.h"
#include "Models/Brand/PastInfluencerCamp.h"

#include <vector>
#include <optional>
#include <cppconn/connection.h>

class PastInfluencerCampRepository {
private:
    DbManager& dbManager;

public:
    PastInfluencerCampRepository(DbManager& db);

    int insert(
        sql::Connection* conn,
        int brandId,
        const std::optional<std::string>& ambassadors,
        const std::optional<std::string>& campaigns,
        const std::optional<std::string>& strategy
    );

    std::vector<PastInfluencerCamp> getByBrandId(int brandId);
    std::optional<PastInfluencerCamp> getById(int picId);

    void update(
        sql::Connection* conn,
        int id,
        const std::optional<std::string>& ambassadors,
        const std::optional<std::string>& campaigns,
        const std::optional<std::string>& strategy
    );

    void softDelete(sql::Connection* conn,
                    int id);
};