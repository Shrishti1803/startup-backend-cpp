#include <iostream>
#include <sodium.h>

#include "Utils/Logger.h"
#include "Auth/AuthService.h"
#include "AppContainer.h"

// Models
#include "Models/Brand/Brand.h"
#include "Models/Brand/Goal.h"
#include "Models/Brand/Competitor.h"

int main() {

    if (sodium_init() < 0) {
        std::cout << "Sodium init failed\n";
        return 1;
    }

    Logger::init();
    auto logger = Logger::app();

    logger->info("Application Started");

    AppContainer app;

    if (!app.db.connect()) {
        logger->error("DB connection failed");
        return 1;
    }

    logger->info("DB connected");

    sql::Connection* conn = app.db.getConnection();

    std::string email = "pqr@gmail.com";
    std::string password = "PQR@password";

    auto session = Authentication(conn, email, password);

    if (!session) {
        logger->error("Authentication failed");
        return 1;
    }

    logger->info("Login successful for {}", session->email);



    logger->info("Application closing");

    return 0;
}