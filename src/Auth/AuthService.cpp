#include<iostream>
#include<string>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include "Auth/AuthService.h"
#include "Security/PasswordHasher.h"
#include "Db/LoginQueries.h"
#include "Utils/Logger.h"


//this function checks about the USER login authentication... TRUE AND FALSE as required will be checked... AND THE SESSION struct will be returned 
std::optional<Session> Authentication(sql::Connection* con,
                const std::string& email, 
                const std::string& password){

        
        auto auth_logger = Logger::auth();
        AdminRecord AR;

        if(!AdminQueryByEmail(con,email,AR)){
            auth_logger->error("Unauthorized Access - Admin not found");
            return std::nullopt;
        }

        int user_id = AR.user_id;


        if(!LoginExistForUser(con,user_id)){
            if(!registerFirstLogin(con,user_id,password)){
                auth_logger->error("First login reg failed");
                return std::nullopt;
                
            }
            auth_logger->info("First login successful");
        }


        else{
            if(!authenticateExistingLogin(con,user_id,password)){
                auth_logger->error("Pass verification failed");
                return std::nullopt;
            }
        }

        //AFTER all the authentication and hashing and all... We create a session struct that 
        //will be used to store the user details in the Session struct..
        //Those details will be used for Role Based Access Control(R.B.A.C) Layer 
        Session session{
            AR.user_id,
            AR.email,
            AR.role
        };

        auth_logger->info("Login Successful");
        return session;
                
}

//First Login... will first hash the entered password, then Inserts the userid and password into the LOGIN table using the INSERTLOGIN function
bool registerFirstLogin(sql::Connection* con, 
    int user_id, const std::string& password){
        std::string hashed = Security::hashPassword(password);
        return InsertLogin(con, user_id, hashed);

}

//Authentication... This will just check if the Password's hash is same as the old stored password hash. Returns true and false accordingly 
bool authenticateExistingLogin(sql::Connection* con,
    int user_id, const std::string& password){
        std::string storedHash;
        if(!GetPasswordHash(con, user_id, storedHash))return false;
        
        else return Security::verifyPassword(password,storedHash);
}