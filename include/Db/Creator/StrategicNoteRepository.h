#pragma once

#include <vector>
#include <optional>

#include "Models/Creator/StrategicNote.h"
#include "Db/DbManager.h"
#include <cppconn/connection.h>

class StrategicNoteRepository {

private:
    DbManager& dbManager;

public:

    StrategicNoteRepository(DbManager& db);

    int insert(
        sql::Connection* conn,
        int creatorId,
        const std::optional<std::string>& description,
        const std::string& author
    );

    std::vector<StrategicNote> getByCreatorId(int creatorId);

    void update(
        sql::Connection* conn,
        int strategicId,
        const std::optional<std::string>& description,
        const std::optional<std::string>& author
    );

    void softDelete(sql::Connection* conn,
                    int strategicId);

    std::optional<StrategicNote> getById(
        int strategicId
    );
};