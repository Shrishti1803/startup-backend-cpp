#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <string>
#include <cppconn/connection.h>

// Main authentication entry point
bool Authentication(sql::Connection* con,
                    const std::string& email,
                    const std::string& password);

// First-time login (returns generated login_id or -1 on failure)
bool registerFirstLogin(sql::Connection* con,
                       int user_id,
                       const std::string& password);

// Existing user authentication
bool authenticateExistingLogin(sql::Connection* con,
                               int user_id,
                               const std::string& password);

#endif
