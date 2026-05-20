#include <iostream>
#include <limits>

#include "CLI/Console.h"
#include "CLI/Query/SearchFilterUI.h"
#include "CLI/Creator/CreatorUI.h"
#include "CLI/DealUI.h"

#include "Db/Analytics/AnalyticsRepository.h"

Console::Console(
    DbManager& db,
    BrandService& brandService,
    CreatorService& creatorService,
    DealService& dealService,
    ReferencesService& refService,
    AnalyticsService& analyticsService
)
    : db(db),
      brandService(brandService),
      creatorService(creatorService),
      referenceService(refService),
      analyticsService(analyticsService),
      dealService(dealService)
{
}

void Console::run()
{
    welcomeMenu();
}

void Console::welcomeMenu()
{
    int choice;

    do
    {
        clearScreen();

        std::cout
            << "\n=================================\n"
            << "            SIMPLIPAL\n"
            << "=================================\n";

        std::cout
            << "1. Login\n"
            << "2. Exit\n";

        std::cout
            << "\nEnter choice: ";

        choice = takeChoice();

        switch(choice)
        {
            case 1:
            {
                loginMenu();
                break;
            }

            case 2:
            {
                std::cout
                    << "\nExiting application...\n";

                break;
            }

            default:
            {
                UI::error(
                    "Invalid choice."
                );

                pause();

                break;
            }
        }

    } while(choice != 2);
}

void Console::loginMenu()
{
    int attempts = 0;

    while(attempts < 3)
    {
        clearScreen();

        std::string email;
        std::string password;

        std::cout
            << "\n=================================\n"
            << "               LOGIN\n"
            << "=================================\n";

        std::cout
            << "Email: ";

        std::getline(
            std::cin,
            email
        );

        if(email.empty())
        {
            UI::error(
                "Email cannot be empty."
            );

            pause();

            continue;
        }

        std::cout
            << "Password: ";

        std::getline(
            std::cin,
            password
        );

        if(password.empty())
        {
            UI::error(
                "Password cannot be empty."
            );

            pause();

            continue;
        }

        try
        {
            auto conn =
                db.getConnection();

            currentSession =
                Auth::Authentication(
                    conn,
                    email,
                    password
                );

            if(currentSession.has_value())
            {
                UI::success(
                    "Login successful."
                );

                pause();

                dashboardMenu();

                return;
            }
        }

        catch(const std::exception& e)
        {
            UI::error(
                e.what()
            );

            pause();

            return;
        }

        attempts++;

        UI::error(
            "Invalid credentials."
        );

        std::cout
            << "Attempts remaining: "
            << (3 - attempts)
            << "\n";

        pause();
    }

    clearScreen();

    std::cout
        << "\nToo many failed attempts.\n\n";

    std::cout
        << "1. Reset Password\n"
        << "0. Back\n";

    int choice =
        takeChoice();

    if(choice == 1)
    {
        std::string email;
        std::string newPassword;

        std::cout
            << "\nEnter registered email: ";

        std::getline(
            std::cin,
            email
        );

        if(email.empty())
        {
            UI::error(
                "Email cannot be empty."
            );

            pause();

            return;
        }

        std::cout
            << "Enter new password: ";

        std::getline(
            std::cin,
            newPassword
        );

        if(newPassword.empty())
        {
            UI::error(
                "Password cannot be empty."
            );

            pause();

            return;
        }

        try
        {
            auto conn =
                db.getConnection();

            if(
                Auth::resetPassword(
                    conn,
                    email,
                    newPassword
                )
            )
            {
                UI::success(
                    "Password reset successful."
                );
            }

            else
            {
                UI::error(
                    "Password reset failed."
                );
            }
        }

        catch(const std::exception& e)
        {
            UI::error(
                e.what()
            );
        }

        pause();
    }
}

void Console::dashboardMenu()
{
    int choice;

    do
    {
        clearScreen();

        std::cout
            << "\n=================================\n"
            << "             DASHBOARD\n"
            << "=================================\n";

        std::cout
            << "1. Brands\n"
            << "2. Creators\n"
            << "3. Deals\n"
            << "4. Reference Management\n"
            << "5. Analytics\n"
            << "6. Search\n"
            << "7. My Profile\n"
            << "8. Logout\n";

        std::cout
            << "\nEnter choice: ";

        choice = takeChoice();

        switch(choice)
        {
            case 1:
            {
                brandsMenu();
                break;
            }

            case 2:
            {
                creatorsMenu();
                break;
            }

            case 3:
            {
                dealsMenu();
                break;
            }

            case 4:
            {
                referenceManagementMenu();
                break;
            }

            case 5:
            {
                analyticsMenu();
                break;
            }

            case 6:
            {
                searchMenu();
                break;
            }

            case 7:
            {
                profileMenu();
                break;
            }

            case 8:
            {
                std::cout
                    << "\nLogging out...\n";

                currentSession.reset();

                pause();

                return;
            }

            default:
            {
                UI::error(
                    "Invalid choice."
                );

                pause();

                break;
            }
        }

    } while(true);
}

void Console::brandsMenu()
{
    if(!currentSession.has_value())
    {
        UI::error(
            "Please login first."
        );

        pause();

        return;
    }

    BrandUI ui(
        brandService,
        referenceService,
        currentSession.value()
    );

    ui.run();
}

void Console::referenceManagementMenu()
{
    if(!currentSession.has_value())
    {
        UI::error(
            "Please login first."
        );

        pause();

        return;
    }

    ReferenceManagementUI ui(
        referenceService,
        currentSession.value()
    );

    ui.run();
}

void Console::creatorsMenu()
{
    if(!currentSession.has_value())
    {
        UI::error(
            "Please login first."
        );

        pause();

        return;
    }

    CreatorUI ui(
        creatorService,
        referenceService,
        currentSession.value()
    );

    ui.run();
}

void Console::dealsMenu()
{
    if(!currentSession.has_value())
    {
        UI::error(
            "Please login first."
        );

        pause();

        return;
    }

    DealUI ui(
        dealService,
        brandService,
        creatorService,
        currentSession.value()
    );

    ui.run();
}

void Console::analyticsMenu()
{
    clearScreen();

    try
    {
        analyticsService.showMonthlyDealsGraph();

        UI::success(
            "Analytics generated successfully."
        );
    }

    catch(const std::exception& e)
    {
        UI::error(
            e.what()
        );
    }

    pause();
}

void Console::searchMenu()
{
    if(!currentSession.has_value())
    {
        UI::error(
            "Please login first."
        );

        pause();

        return;
    }

    BrandUI brandUI(
        brandService,
        referenceService,
        currentSession.value()
    );

    CreatorUI creatorUI(
        creatorService,
        referenceService,
        currentSession.value()
    );

    SearchFilterUI ui(
        brandService,
        creatorService,
        referenceService,
        brandUI,
        creatorUI,
        currentSession.value()
    );

    ui.run();
}

void Console::profileMenu()
{
    if(!currentSession.has_value())
    {
        UI::error(
            "Please login first."
        );

        pause();

        return;
    }

    ProfileUI ui(
        db,
        currentSession.value()
    );

    ui.run();
}

int Console::takeChoice()
{
    int choice;

    while(!(std::cin >> choice))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        UI::error(
            "Enter a valid number."
        );

        std::cout
            << "Choice: ";
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    return choice;
}

void Console::pause()
{
    std::cout
        << "\nPress Enter to continue...";

    std::cin.get();
}