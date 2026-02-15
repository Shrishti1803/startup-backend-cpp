#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <iostream>
#include<vector>
#include <string>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>

class DbManager{
    public :
    DbManager();
    ~DbManager();
    bool connect();
    sql::Connection* getConnection();


    private :
    sql::Connection* conn;
};

#endif