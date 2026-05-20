#include "Db/DbManager.h"
#include "Utils/Logger.h"

#include <mysql_driver.h>
#include <mysql_connection.h>

#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>

#include <iostream>
#include <stdexcept>

// THIS file has all the necessary functions to establish the DB connection
// ONLY THIS FILE WILL GIVE YOU THE ACCESS TO THE DB
// DO NOT DARE TO ASK FOR SEPARATE CONNECTIONS WHEN I HAVE LITERALLY MADE THESE HELPER FUNCTIONS
// THIS IS THE SOUL OF THE DB CONNECTION DO NOT WASTE IT OR TAKE IT FOR GRANTED

bool DbManager::connect()
{
    auto db_logger = Logger::db();

    try
    {
        sql::Driver* driver = get_driver_instance();

        conn = driver->connect(
            "Db Host server",
            "root",
            "Db Password"
        );

        conn->setSchema("Database name");

        if (db_logger)
        {
            db_logger->info(
                "Connected to database schema: Database"
            );
        }

        return true;
    }
    catch (sql::SQLException& e)
    {
        conn = nullptr;

        std::cerr
            << "MySQL Error: "
            << e.what()
            << std::endl;

        if (db_logger)
        {
            db_logger->critical(
                "Database connection failed: {}",
                e.what()
            );
        }

        return false;
    }
}

sql::Connection* DbManager::getConnection()
{
    return conn;
}

// Constructor
DbManager::DbManager() : conn(nullptr)
{
    auto db_logger = Logger::db();

    if (connect())
    {
        if (db_logger)
        {
            db_logger->info(
                "DbManager initialized successfully."
            );
        }
    }
    else
    {
        if (db_logger)
        {
            db_logger->critical(
                "DbManager failed to initialize."
            );
        }

        throw std::runtime_error(
            "Failed to connect to database"
        );
    }
}

// Destructor
DbManager::~DbManager()
{
    auto db_logger = Logger::db();

    if (conn)
    {
        delete conn;
        conn = nullptr;
    }

    if (db_logger)
    {
        db_logger->info(
            "Database connection closed"
        );
    }
}