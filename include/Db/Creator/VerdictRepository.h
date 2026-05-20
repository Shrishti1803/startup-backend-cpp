#pragma once

#include <optional>

#include "Models/Creator/Verdict.h"
#include "Db/DbManager.h"
#include <cppconn/connection.h>

class VerdictRepository {

private:
    DbManager& dbManager;

public:

    VerdictRepository(DbManager& db);

    int insert(
        sql::Connection* conn,
        int creatorId,
        const std::optional<std::string>& strengths,
        const std::optional<std::string>& weaknesses,
        const std::optional<std::string>& growth,
        const std::optional<std::string>& finalVerdict
    );

    std::optional<Verdict> getByCreatorId(int creatorId);

    void update(
        sql::Connection* conn,
        int verdictId,
        const std::optional<std::string>& strengths,
        const std::optional<std::string>& weaknesses,
        const std::optional<std::string>& growth,
        const std::optional<std::string>& finalVerdict
    );

    void softDelete(sql::Connection* conn,
                    int verdictId);
};