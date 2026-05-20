#pragma once

#include <vector>
#include <optional>
#include "../../Models/Brand/HeadContact.h"
#include "../DbManager.h"
#include <cppconn/connection.h>

class HeadContactRepository {
private:
    DbManager& dbManager;

public:
    explicit HeadContactRepository(DbManager& db);

    int insert(sql::Connection* conn,
               int headId,
               const std::optional<std::string>& contactType,
               const std::string& contactValue);

    std::vector<HeadContact> getByHeadId(int headId);
    std::optional<HeadContact> getById(int contactId);

    void update(sql::Connection* conn,
                int contactId,
                const std::optional<std::string>& contactType,
                const std::optional<std::string>& contactValue);

    void softDelete(sql::Connection* conn,
                    int contactId);
};