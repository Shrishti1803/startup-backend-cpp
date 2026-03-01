#pragma once

#include <vector>
#include "../../Models/Brand/NewsFunding.h"
#include "Models/Helpers/Date.h"
#include "../DbManager.h"

class NewsFundingRepository {
private:
    DbManager& dbManager;

public:
    explicit NewsFundingRepository(DbManager& db);

    int insert(int brandId,
               const std::optional<std::string>& eventType,
               const std::optional<std::string>& title,
               const std::optional<std::string>& description,
               const std::optional<Date>& eventDate);

    std::vector<NewsFunding> getByBrandId(int brandId);

    void update(int newsId,
            const std::optional<std::string>& eventType,
            const std::optional<std::string>& title,
            const std::optional<std::string>& description,
            const std::optional<Date>& eventDate);

    void softDelete(int newsId);

};