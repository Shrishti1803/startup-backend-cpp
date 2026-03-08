#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <string>
#include <optional>
#include <cppconn/connection.h>

//This Struct is used to store these details as the user enters 
//SO THAT ROLE BASED ACCESS CONTROL CAN BE APPLIED TO THE FUNCTIONS THROUGHOUT THE APPLICATION
struct Session{
    int user_id;
    std::string email;
    std::string role;

};
// Main authentication entry point
std::optional<Session> Authentication(sql::Connection* con,
                    const std::string& email,
                    const std::string& password);

//Returns true when success and false when not IN CASE OF FIRST LOGIN 
bool registerFirstLogin(sql::Connection* con,
                       int user_id,
                       const std::string& password);

// Existing user authentication .... Will be used to verify if the existing user_id password is verified accordingly from the DB 
bool authenticateExistingLogin(sql::Connection* con,
                               int user_id,
                               const std::string& password);


                            


#endif
