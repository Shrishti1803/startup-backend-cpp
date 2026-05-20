#pragma once

#include <vector>
#include <optional>

#include "Models/Creator/ProfileAccount.h"
#include "Db/DbManager.h"
#include <cppconn/connection.h>

class ProfileAccountRepository
{

private:

    DbManager& dbManager;

public:

    ProfileAccountRepository(DbManager& db);



    //CREATE 

    int insert(
        sql::Connection* conn,
        int creator_id,
        int platform_id,
        const std::string& link,
        const std::optional<long long>& followers
    );



   //READ

    ProfileAccount getById(int profile_id);

    std::vector<ProfileAccount>
    getByCreatorId(int creator_id);

    std::vector<ProfileAccount> getAll();



   //Patch Update

    void update(
        sql::Connection* conn,
        int profile_id,
        const std::optional<int>& platform_id,
        const std::optional<std::string>& link,
        const std::optional<long long>& followers
    );



    //Soft delete

    void softDelete(sql::Connection* conn,
                    int profile_id);


    std::optional<ProfileAccount>
        getByLink(const std::string& link);

};