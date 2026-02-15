#ifndef LOGIN_QUERIES_H
#define LOGIN_QUERIES_H
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/connection.h>

#include<string>

struct LoginRecord {
    int login_id;
    int user_id;
    std::string email;
    std::string password_hash;
    bool is_active;
};

bool InsertLogin(sql::Connection* con,
                int user_id, 
                const std::string& password_hash);

struct AdminRecord{
    int user_id;
    std::string email;
    std::string role;
};

bool AdminQueryByEmail(sql::Connection* con, 
    const std::string& email,
    AdminRecord& outAdmin
);

bool GetPasswordHash(sql::Connection* con, int user_id,
    std::string& outHash);



bool LoginExistForUser(sql::Connection* con, int user_id);

#endif