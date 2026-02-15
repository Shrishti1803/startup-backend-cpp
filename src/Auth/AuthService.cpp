#include<iostream>
#include<string>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include "Auth/AuthService.h"
#include "Security/PasswordHasher.h"
#include "Db/LoginQueries.h"
#include "Utils/Logger.h"

bool Authentication(sql::Connection* con,
                const std::string& email, 
                const std::string& password){

        
        auto auth_logger = Logger::auth();
        AdminRecord AR;

        if(!AdminQueryByEmail(con,email,AR)){
            auth_logger->error("Unauthorized Access - Admin not found");
            return false;
        }

        int user_id = AR.user_id;

        if(!LoginExistForUser(con,user_id)){
            if(!registerFirstLogin(con,user_id,password)){
                auth_logger->error("First login reg failed");
                return false;
            }
            auth_logger->info("First login successful");
            return true;
        }


        if(!authenticateExistingLogin(con,user_id,password)){
            auth_logger->error("Pass verification failed");
            return false;
        }

        auth_logger->info("Login Successful");
        return true;
                
}

bool registerFirstLogin(sql::Connection* con, 
    int user_id, const std::string& password){
        std::string hashed = Security::hashPassword(password);
        return InsertLogin(con, user_id, hashed);

}

bool authenticateExistingLogin(sql::Connection* con,
    int user_id, const std::string& password){
        std::string storedHash;
        if(!GetPasswordHash(con, user_id, storedHash))return false;
        
        else return Security::verifyPassword(password,storedHash);
}