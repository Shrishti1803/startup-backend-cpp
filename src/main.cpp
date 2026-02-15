#include "Utils/Logger.h"
#include<iostream>
#include "Db/DbManager.h"
#include "Auth/AuthService.h"
#include <sodium.h>


int main() {
    if (sodium_init() < 0) {
        return 1;
    }

    Logger::init();
    auto app_logger = Logger::app();
    app_logger->info("Application Started!");

    DbManager DB;
    
    if(! DB.connect()){
        app_logger->error("DB Connection failed, exiting application");
        return 1;
    }

    app_logger->info("DB connection verified in main");

    std::string email = "sample@email.com";
    std::string pass = "samplePassword";

    sql::Connection* conn  = DB.getConnection();
    std::cout << Authentication(conn,email,pass);

    app_logger->info("Closing the application");

    return 0;
}
