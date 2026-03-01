#pragma once

#include <optional>
#include "Models/Brand/Standard.h"
#include "../DbManager.h"

class BrandStandardRepository {
private:
    DbManager& dbManager;

public:
    explicit BrandStandardRepository(DbManager& db);

    int insert(int brandId,
               const std::optional<std::string>& position,
               const std::optional<std::string>& identity,
               const std::optional<std::string>& csr,
               const std::optional<std::string>& quality,
               const std::optional<std::string>& perception);

    std::optional<Standard> getByBrandId(int brandId);

        void update(int standardId,
                const std::optional<std::string>& position,
                const std::optional<std::string>& identity,
                const std::optional<std::string>& csr,
                const std::optional<std::string>& quality,
                const std::optional<std::string>& perception);

    void softDelete(int standard_id);
};