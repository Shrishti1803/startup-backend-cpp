#pragma once

#include <vector>
#include <optional>

#include "Models/Creator/HPVAudit.h"
#include "Db/DbManager.h"
#include <cppconn/connection.h>

class HPVAuditRepository {

private:
    DbManager& dbManager;

public:

    HPVAuditRepository(DbManager& db);

    int insert(
        sql::Connection* conn,
        int creatorId,
        const std::optional<std::string>& topic,
        const std::optional<std::string>& spikeRate
    );

    std::vector<HPVAudit> getByCreatorId(int creatorId);

    void update(
        sql::Connection* conn,
        int hpvId,
        const std::optional<std::string>& topic,
        const std::optional<std::string>& spikeRate
    );

    void softDelete(sql::Connection* conn,
                    int hpvId);

    std::optional<HPVAudit> getById(int hpvId);
};