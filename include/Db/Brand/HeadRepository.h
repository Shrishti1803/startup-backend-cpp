#pragma once

#include <vector>
#include <optional>
#include "../../Models/Brand/BrandHead.h"
#include "../DbManager.h"
#include <cppconn/connection.h>

class HeadRepository {
private:
    DbManager& dbManager;

public:
    explicit HeadRepository(DbManager& db);

    int insert(sql::Connection* conn,
               int brandId,
               int positionId,
               const std::string& name,
               const std::optional<std::string>& linkedinId,
               const std::string& personalMail,
               const std::string& officialMail);

    std::vector<BrandHead> getByBrandId(int brandId);
    std::optional<BrandHead> getById(int headId);

    void update(
        sql::Connection* conn,
        int headId,
        const std::optional<int>& positionId,
        const std::optional<std::string>& name,
        const std::optional<std::string>& linkedinId,
        const std::optional<std::string>& personalMail,
        const std::optional<std::string>& officialMail
    );

    void softDelete(sql::Connection* conn,
                    int headId);
};