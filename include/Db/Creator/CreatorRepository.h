#pragma once

#include "Db/DbManager.h"
#include "Models/Creator/Creator.h"

#include <vector>
#include <optional>
#include <cppconn/connection.h>

class CreatorRepository {

private:

    DbManager& dbManager;

public:

    explicit CreatorRepository(
        DbManager& db
    );



    // CREATE

    int insert(
        sql::Connection* conn,
        const std::string& name,
        const std::optional<std::string>& email,
        const std::optional<std::string>& rating,
        const std::optional<long long>& audienceSize
    );



    //READ

    Creator getById(
        int creatorId
    );

    std::vector<Creator> getAll();



    // UPDATE

    void updateName(
        sql::Connection* conn,
        int creatorId,
        const std::optional<std::string>& name
    );

    void updateEmail(
        sql::Connection* conn,
        int creatorId,
        const std::optional<std::string>& email
    );

    void updateRating(
        sql::Connection* conn,
        int creatorId,
        const std::optional<std::string>& rating
    );

    void updateAudienceSize(
        sql::Connection* conn,
        int creatorId,
        const std::optional<long long>& audienceSize
    );



    //DELETE

    void softDelete(
        sql::Connection* conn,
        int creatorId
    );



    // VALIDATION

    bool existsById(
        sql::Connection* conn,
        int creatorId
    );
};