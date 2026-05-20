#pragma once

#include <optional>
#include "Models/Brand/Standard.h"
#include "../DbManager.h"
#include <cppconn/connection.h>

class BrandStandardRepository {
private:
    DbManager& dbManager;

public:
    explicit BrandStandardRepository(DbManager& db);

    int insert(sql::Connection* conn,
               int brandId,
               const std::optional<std::string>& position,
               const std::optional<std::string>& identity,
               const std::optional<std::string>& csr,
               const std::optional<std::string>& quality,
               const std::optional<std::string>& perception);

    std::optional<Standard> getByBrandId(int brandId);

    void update(sql::Connection* conn,
                int standardId,
                const std::optional<std::string>& position,
                const std::optional<std::string>& identity,
                const std::optional<std::string>& csr,
                const std::optional<std::string>& quality,
                const std::optional<std::string>& perception);

    void softDelete(sql::Connection* conn,
                    int standardId);
};