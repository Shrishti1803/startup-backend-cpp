#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <iostream>
#include<vector>
#include <string>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>

//Declaration of all the functions required to establish a successful DB connection in the CLASS 
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