#pragma once

#include <vector>
#include <optional>
#include "../../Models/Brand/NewsFunding.h"
#include "Models/Helpers/Date.h"
#include "../DbManager.h"
#include <cppconn/connection.h>

class NewsFundingRepository {
private:
    DbManager& dbManager;

public:
    explicit NewsFundingRepository(DbManager& db);

    int insert(sql::Connection* conn,
               int brandId,
               const std::optional<std::string>& eventType,
               const std::optional<std::string>& title,
               const std::optional<std::string>& description,
               const std::optional<Date>& eventDate);

    std::vector<NewsFunding> getByBrandId(int brandId);
    std::optional<NewsFunding> getById(int newsFundingId);

    void update(sql::Connection* conn,
                int newsId,
                const std::optional<std::string>& eventType,
                const std::optional<std::string>& title,
                const std::optional<std::string>& description,
                const std::optional<Date>& eventDate);

    void softDelete(sql::Connection* conn,
                    int newsId);
};