#include "CLI/ProfileUI.h"

#include <iostream>
#include <limits>

ProfileUI::ProfileUI(
    DbManager& db,
    const Auth::Session& session
)
    : db(db),
      currentSession(session)
{
}

void ProfileUI::run()
{
    int choice;

    do
    {
        clearScreen();

        showProfile();

        std::cout
            << "\n1. Change Password\n"
            << "0. Back\n";

        std::cout
            << "\nEnter choice: ";

        choice = takeChoice();

        switch(choice)
        {
            case 1:
            {
                changePasswordFlow();
                break;
            }

            case 0:
            {
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

void ProfileUI::showProfile()
{
    std::cout
        << "\n=================================\n";

    std::cout
        << "           MY PROFILE\n";

    std::cout
        << "=================================\n";

    std::cout
        << " Email : "
        << currentSession.email
        << '\n';

    std::cout
        << " Role  : "
        << currentSession.role
        << '\n';
}

void ProfileUI::changePasswordFlow()
{
    clearScreen();

    std::string currentPassword;
    std::string newPassword;
    std::string confirmPassword;

    std::cout
        << "\n=================================\n";

    std::cout
        << "         CHANGE PASSWORD\n";

    std::cout
        << "=================================\n";

    std::cout
        << "\nCurrent Password: ";

    std::getline(
        std::cin,
        currentPassword
    );

    if(currentPassword.empty())
    {
        UI::error(
            "Current password cannot be empty."
        );

        pause();

        return;
    }

    std::cout
        << "New Password: ";

    std::getline(
        std::cin,
        newPassword
    );

    if(newPassword.empty())
    {
        UI::error(
            "New password cannot be empty."
        );

        pause();

        return;
    }

    if(newPassword.length() < 6)
    {
        UI::error(
            "Password must be at least 6 characters."
        );

        pause();

        return;
    }

    std::cout
        << "Confirm Password: ";

    std::getline(
        std::cin,
        confirmPassword
    );

    if(confirmPassword.empty())
    {
        UI::error(
            "Confirmation password cannot be empty."
        );

        pause();

        return;
    }

    if(newPassword != confirmPassword)
    {
        UI::error(
            "Passwords do not match."
        );

        pause();

        return;
    }

    if(currentPassword == newPassword)
    {
        UI::error(
            "New password cannot be same as current password."
        );

        pause();

        return;
    }

    try
    {
        auto conn =
            db.getConnection();

        bool success =
            Auth::changePassword(
                conn,
                currentSession.user_id,
                currentPassword,
                newPassword
            );

        if(success)
        {
            UI::success(
                "Password changed successfully."
            );
        }

        else
        {
            UI::error(
                "Current password incorrect."
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

int ProfileUI::takeChoice()
{
    int choice;

    while(!(std::cin >> choice))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<
                std::streamsize
            >::max(),
            '\n'
        );

        std::cout
            << "Enter valid number: ";
    }

    std::cin.ignore(
        std::numeric_limits<
            std::streamsize
        >::max(),
        '\n'
    );

    return choice;
}

void ProfileUI::pause()
{
    std::cout
        << "\nPress Enter to continue...";

    std::cin.get();
}