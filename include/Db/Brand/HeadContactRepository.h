#pragma once

#include <vector>
#include <optional>
#include "../../Models/Brand/HeadContact.h"
#include "../DbManager.h"

class HeadContactRepository {
private:
    DbManager& dbManager;

public:
    explicit HeadContactRepository(DbManager& db);

    int insert(int headId,
               const std::optional<std::string>& contactType,
               const std::string& contactValue);

    std::vector<HeadContact> getByHeadId(int headId);

    void update(int contactId,
                const std::optional<std::string>& contactType,
                const std::optional<std::string>& contactValue);    
    void softDelete(int contactId);

};