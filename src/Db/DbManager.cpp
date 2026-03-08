#include "Db/DbManager.h"
#include "Utils/Logger.h"

#include <mysql_driver.h>
#include <mysql_connection.h>

#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>

//THIS file has all the necessary functions to eastablish the DB connection
//ONLY THIS FILE WILL GIVE YOU THE ACCESS TO THE DB 
//DO NOT DARE TO ASK FOR SEPARATE CONNECTIONS WHEN I HAVE LITERALLY MADE THESE HELPER FUNCTIONS
//THIS IS THE SOUL OF THE DB CONNECTION DO NOT WASTE IT OR TAKE IT FOR GRANTED 

bool DbManager::connect(){

    auto db_logger = Logger::db();
    db_logger->info("Attempt to connect to database");
    try {
        sql::Driver* driver = get_driver_instance();
        conn = driver->connect("tcp://127.0.0.1:3306", "root", "shrishti@2006");  

        conn->setSchema("SIMLIPAL"); 


        db_logger->info("Database connection successful");
        return true;
        //now when connection is established.... conn is updated to the required value 

    } 

    catch (sql::SQLException &e) {
        db_logger->error("Database connection failed : {}", e.what());
        conn = nullptr;
        //but if connection is not established.. conn is set to nullptr
        
        return false;
    }
}

sql::Connection* DbManager::getConnection(){
    return conn; //to make the required connection we call this function and this function will return the value of conn if the connection is successfully established 
}

DbManager::DbManager() : conn(nullptr){}

//Deconstructor :
DbManager::~DbManager(){

    auto db_logger = Logger::db();
    if(conn){ // now if connection was successfully established... and since we using classes.. we also need to make the deconstructor if(conn) .. i.e if(true)
        delete conn; //hence we delete the conn
        conn = nullptr; // and assign nullptr to conn 
    }
    db_logger->info("Database connection closed");
}
