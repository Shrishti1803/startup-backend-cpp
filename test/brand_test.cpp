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

    int brandId = 1;   // existing brand id for testing



    // ==============================
    // CREATE BRAND
    // ==============================

    try {

        BrandAggregate data;

        Brand brand(0, "Backend Test Brand");
        data.brand = brand;

        Goal goal;
        goal.setDescription("Expand global presence");
        data.goals.push_back(goal);

        Competitor competitor;
        competitor.setName("Competitor Example");
        data.competitors.push_back(competitor);

        app.brandService.createBrand(*session, data);

        logger->info("Brand created successfully");

    } catch (const std::exception& e) {

        logger->error("Brand creation failed: {}", e.what());

    }



    // ==============================
    // UPDATE BRAND
    // ==============================

    try {

        BrandAggregate updateData;

        Brand updatedBrand(brandId, "Updated Brand Name");
        updateData.brand = updatedBrand;

        app.brandService.updateBrand(*session, brandId, updateData);

        logger->info("Brand updated successfully");

    } catch (const std::exception& e) {

        logger->error("Brand update failed: {}", e.what());

    }



    // ==============================
    // GET BRAND BY ID
    // ==============================

    try {

        auto brandData = app.brandService.getBrandById(*session, brandId);

        logger->info("Fetched Brand: {}", brandData.brand.getName());

    } catch (const std::exception& e) {

        logger->error("Fetching brand failed: {}", e.what());

    }



    // ==============================
    // DELETE BRAND
    // ==============================

    try {

        app.brandService.deleteBrand(*session, brandId);

        logger->info("Brand soft deleted successfully");

    } catch (const std::exception& e) {

        logger->error("Brand deletion failed: {}", e.what());

    }



    logger->info("Application closing");

    return 0;
}