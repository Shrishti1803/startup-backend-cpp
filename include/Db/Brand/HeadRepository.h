#pragma once

#include <vector>
#include "../../Models/Brand/Head.h"
#include "../DbManager.h"

class HeadRepository {
private:
    DbManager& dbManager;

public:
    explicit HeadRepository(DbManager& db);

    int insert(int brandId,
               int positionId,
               const std::string& name,
               const std::optional<std::string>& linkedinId,
               const std::string& personalMail,
               const std::string& officialMail);

    std::vector<Head> getByBrandId(int brandId);


    void update(
    int headId,
    const std::optional<int>& positionId,
    const std::optional<std::string>& name,
    const std::optional<std::string>& linkedinId,
    const std::optional<std::string>& personalMail,
    const std::optional<std::string>& officialMail);

    void softDelete(int headId);

};