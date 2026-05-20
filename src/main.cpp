#include <iostream>
#include <exception>
#include "Utils/Logger.h"

#include "AppContainer.h"
#include "CLI/Console.h"

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _WIN32
void maximizeConsoleWindow()
{
    HWND consoleWindow =
        GetConsoleWindow();

    ShowWindow(
        consoleWindow,
        SW_MAXIMIZE
    );
}
#endif

int main()
{
#ifdef _WIN32
    maximizeConsoleWindow();
#endif

    try
    {
        Logger::init();

        auto log = Logger::app();

        if (log)
            log->info("Application starting...");

        AppContainer app;

        if (log)
            log->info("AppContainer initialized.");

        // Database connection test
        auto conn = app.db.getConnection();

        if (!conn)
        {
            if (log)
                log->critical("DbManager returned nullptr connection.");

            std::cerr
                << "Database connection failed.\n"
                << "Check:\n"
                << "1. MySQL service running\n"
                << "2. DB username/password\n"
                << "3. Schema name\n"
                << "4. MySQL connector setup\n";

            return 1;
        }

        if (log)
            log->info("Database connected successfully.");

        Console console(
            app.db,
            app.brandService,
            app.creatorService,
            app.dealService,
            app.referencesService,
            app.analyticsService
        );

        if (log)
            log->info("Launching CLI...");

        console.run();
    }
    catch (const std::exception& ex)
    {
        std::cerr
            << "Fatal Error: "
            << ex.what()
            << std::endl;

        auto log = Logger::app();

        if (log)
        {
            log->critical(
                "Fatal exception: {}",
                ex.what()
            );
        }
    }
    catch (...)
    {
        std::cerr
            << "Unknown fatal error occurred."
            << std::endl;

        auto log = Logger::app();

        if (log)
        {
            log->critical(
                "Unknown fatal exception occurred."
            );
        }
    }

    return 0;
}