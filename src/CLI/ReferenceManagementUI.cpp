#include "CLI/ReferenceManagementUI.h"

#include <iostream>
#include <limits>
#include <optional>

ReferenceManagementUI::ReferenceManagementUI(
    ReferencesService& service,
    const Auth::Session& session
)
    : referencesService(service),
      currentSession(session)
{
}

void ReferenceManagementUI::run()
{
    int choice;

    do
    {
        clearScreen();

        std::cout
            << "\n=================================\n";

        std::cout
            << "      REFERENCE MANAGEMENT\n";

        std::cout
            << "=================================\n";

        std::cout
            << "1. Genres\n"
            << "2. Target Audience\n"
            << "3. Niches\n"
            << "4. Platforms\n"
            << "5. Positions\n"
            << "6. Back\n";

        std::cout
            << "\nEnter choice: ";

        choice = takeChoice();

        switch(choice)
        {
            case 1:
                genresMenu();
                break;

            case 2:
                targetAudienceMenu();
                break;

            case 3:
                nichesMenu();
                break;

            case 4:
                platformsMenu();
                break;

            case 5:
                positionsMenu();
                break;

            case 6:
                return;

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

// =====================================================
// GENRES
// =====================================================

void ReferenceManagementUI::genresMenu()
{
    int choice;

    do
    {
        clearScreen();

        auto genres =
            referencesService.getAllGenres(
                currentSession
            );

        std::cout
            << "\n=================================\n";

        std::cout
            << "              GENRES\n";

        std::cout
            << "=================================\n";

        if(genres.empty())
        {
            std::cout
                << "No genres found.\n";
        }

        else
        {
            for(const auto& genre : genres)
            {
                std::cout
                    << "["
                    << genre.getId()
                    << "] "
                    << genre.getCategory()
                    << "\n";
            }
        }

        std::cout
            << "\n1. Add Genre\n"
            << "2. Rename Genre\n"
            << "3. Delete Genre\n"
            << "4. Back\n";

        std::cout
            << "\nEnter choice: ";

        choice = takeChoice();

        switch(choice)
        {
            case 1:
                addGenre();
                break;

            case 2:
                renameGenre();
                break;

            case 3:
                deleteGenre();
                break;

            case 4:
                return;

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

void ReferenceManagementUI::addGenre()
{
    clearScreen();

    std::string category;

    std::cout
        << "\nEnter Genre Name: ";

    std::getline(
        std::cin,
        category
    );

    if(category.empty())
    {
        UI::error(
            "Genre name cannot be empty."
        );

        pause();

        return;
    }

    try
    {
        referencesService.createGenre(
            currentSession,
            category
        );

        UI::success(
            "Genre added successfully."
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

void ReferenceManagementUI::renameGenre()
{
    clearScreen();

    int genreId;

    std::cout
        << "\nEnter Genre ID to rename: ";

    genreId = takeChoice();

    std::string newName;

    std::cout
        << "Enter New Genre Name: ";

    std::getline(
        std::cin,
        newName
    );

    if(newName.empty())
    {
        UI::error(
            "Genre name cannot be empty."
        );

        pause();

        return;
    }

    try
    {
        referencesService.updateGenre(
            currentSession,
            genreId,
            newName,
            std::nullopt
        );

        UI::success(
            "Genre updated successfully."
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

void ReferenceManagementUI::deleteGenre()
{
    clearScreen();

    int genreId;

    std::cout
        << "\nEnter Genre ID to delete: ";

    genreId = takeChoice();

    try
    {
        referencesService.deleteGenre(
            currentSession,
            genreId
        );

        UI::success(
            "Genre deleted successfully."
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

// =====================================================
// TARGET AUDIENCE
// =====================================================

void ReferenceManagementUI::targetAudienceMenu()
{
    int choice;

    do
    {
        clearScreen();

        auto audiences =
            referencesService.getAllTargetAudience(
                currentSession
            );

        std::cout
            << "\n=================================\n";

        std::cout
            << "         TARGET AUDIENCE\n";

        std::cout
            << "=================================\n";

        if(audiences.empty())
        {
            std::cout
                << "No target audience found.\n";
        }

        else
        {
            for(const auto& audience : audiences)
            {
                std::cout
                    << "["
                    << audience.getId()
                    << "] "
                    << audience.getName()
                    << "\n";
            }
        }

        std::cout
            << "\n1. Add Target Audience\n"
            << "2. Rename Target Audience\n"
            << "3. Delete Target Audience\n"
            << "4. Back\n";

        std::cout
            << "\nEnter choice: ";

        choice = takeChoice();

        switch(choice)
        {
            case 1:
                addTargetAudience();
                break;

            case 2:
                renameTargetAudience();
                break;

            case 3:
                deleteTargetAudience();
                break;

            case 4:
                return;

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

void ReferenceManagementUI::addTargetAudience()
{
    clearScreen();

    std::string name;

    std::cout
        << "\nEnter Target Audience Name: ";

    std::getline(
        std::cin,
        name
    );

    if(name.empty())
    {
        UI::error(
            "Target audience name cannot be empty."
        );

        pause();

        return;
    }

    try
    {
        referencesService.createTargetAudience(
            currentSession,
            name
        );

        UI::success(
            "Target audience added successfully."
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

void ReferenceManagementUI::renameTargetAudience()
{
    clearScreen();

    int audId;

    std::cout
        << "\nEnter Target Audience ID to rename: ";

    audId = takeChoice();

    std::string newName;

    std::cout
        << "Enter New Target Audience Name: ";

    std::getline(
        std::cin,
        newName
    );

    if(newName.empty())
    {
        UI::error(
            "Target audience name cannot be empty."
        );

        pause();

        return;
    }

    try
    {
        referencesService.updateTargetAudience(
            currentSession,
            audId,
            newName
        );

        UI::success(
            "Target audience updated successfully."
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

void ReferenceManagementUI::deleteTargetAudience()
{
    clearScreen();

    int audId;

    std::cout
        << "\nEnter Target Audience ID to delete: ";

    audId = takeChoice();

    try
    {
        referencesService.deleteTargetAudience(
            currentSession,
            audId
        );

        UI::success(
            "Target audience deleted successfully."
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

// =====================================================
// NICHES
// =====================================================

void ReferenceManagementUI::nichesMenu()
{
    int choice;

    do
    {
        clearScreen();

        auto niches =
            referencesService.getAllNiches(
                currentSession
            );

        std::cout
            << "\n=================================\n";

        std::cout
            << "              NICHES\n";

        std::cout
            << "=================================\n";

        if(niches.empty())
        {
            std::cout
                << "No niches found.\n";
        }

        else
        {
            for(const auto& niche : niches)
            {
                std::cout
                    << "["
                    << niche.getId()
                    << "] "
                    << niche.getType()
                    << "\n";
            }
        }

        std::cout
            << "\n1. Add Niche\n"
            << "2. Rename Niche\n"
            << "3. Delete Niche\n"
            << "4. Back\n";

        std::cout
            << "\nEnter choice: ";

        choice = takeChoice();

        switch(choice)
        {
            case 1:
                addNiche();
                break;

            case 2:
                renameNiche();
                break;

            case 3:
                deleteNiche();
                break;

            case 4:
                return;

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

void ReferenceManagementUI::addNiche()
{
    clearScreen();

    std::string name;

    std::cout
        << "\nEnter Niche Name: ";

    std::getline(
        std::cin,
        name
    );

    if(name.empty())
    {
        UI::error(
            "Niche name cannot be empty."
        );

        pause();

        return;
    }

    try
    {
        referencesService.createNiche(
            currentSession,
            name
        );

        UI::success(
            "Niche added successfully."
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

void ReferenceManagementUI::renameNiche()
{
    clearScreen();

    int nicheId;

    std::cout
        << "\nEnter Niche ID to rename: ";

    nicheId = takeChoice();

    std::string newName;

    std::cout
        << "Enter New Niche Name: ";

    std::getline(
        std::cin,
        newName
    );

    if(newName.empty())
    {
        UI::error(
            "Niche name cannot be empty."
        );

        pause();

        return;
    }

    try
    {
        referencesService.updateNiche(
            currentSession,
            nicheId,
            newName
        );

        UI::success(
            "Niche updated successfully."
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

void ReferenceManagementUI::deleteNiche()
{
    clearScreen();

    int nicheId;

    std::cout
        << "\nEnter Niche ID to delete: ";

    nicheId = takeChoice();

    try
    {
        referencesService.deleteNiche(
            currentSession,
            nicheId
        );

        UI::success(
            "Niche deleted successfully."
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

// =====================================================
// PLATFORMS
// =====================================================

void ReferenceManagementUI::platformsMenu()
{
    int choice;

    do
    {
        clearScreen();

        auto platforms =
            referencesService.getAllPlatforms(
                currentSession
            );

        std::cout
            << "\n=================================\n";

        std::cout
            << "             PLATFORMS\n";

        std::cout
            << "=================================\n";

        if(platforms.empty())
        {
            std::cout
                << "No platforms found.\n";
        }

        else
        {
            for(const auto& p : platforms)
            {
                std::cout
                    << "["
                    << p.getId()
                    << "] "
                    << p.getName()
                    << "\n";
            }
        }

        std::cout
            << "\n1. Add Platform\n"
            << "2. Rename Platform\n"
            << "3. Delete Platform\n"
            << "4. Back\n";

        std::cout
            << "\nEnter choice: ";

        choice = takeChoice();

        switch(choice)
        {
            case 1:
                addPlatform();
                break;

            case 2:
                renamePlatform();
                break;

            case 3:
                deletePlatform();
                break;

            case 4:
                return;

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

void ReferenceManagementUI::addPlatform()
{
    clearScreen();

    std::string name;

    std::cout
        << "\nEnter Platform Name: ";

    std::getline(
        std::cin,
        name
    );

    if(name.empty())
    {
        UI::error(
            "Platform name cannot be empty."
        );

        pause();

        return;
    }

    try
    {
        referencesService.createPlatform(
            currentSession,
            name
        );

        UI::success(
            "Platform added successfully."
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

void ReferenceManagementUI::renamePlatform()
{
    clearScreen();

    int platformId;

    std::cout
        << "\nEnter Platform ID to rename: ";

    platformId = takeChoice();

    std::string newName;

    std::cout
        << "Enter New Platform Name: ";

    std::getline(
        std::cin,
        newName
    );

    if(newName.empty())
    {
        UI::error(
            "Platform name cannot be empty."
        );

        pause();

        return;
    }

    try
    {
        referencesService.updatePlatform(
            currentSession,
            platformId,
            newName
        );

        UI::success(
            "Platform updated successfully."
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

void ReferenceManagementUI::deletePlatform()
{
    clearScreen();

    int platformId;

    std::cout
        << "\nEnter Platform ID to delete: ";

    platformId = takeChoice();

    try
    {
        referencesService.deletePlatform(
            currentSession,
            platformId
        );

        UI::success(
            "Platform deleted successfully."
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

// =====================================================
// POSITIONS
// =====================================================

void ReferenceManagementUI::positionsMenu()
{
    int choice;

    do
    {
        clearScreen();

        auto positions =
            referencesService.getAllPositions(
                currentSession
            );

        std::cout
            << "\n=================================\n";

        std::cout
            << "             POSITIONS\n";

        std::cout
            << "=================================\n";

        if(positions.empty())
        {
            std::cout
                << "No positions found.\n";
        }

        else
        {
            for(const auto& p : positions)
            {
                std::cout
                    << "["
                    << p.getId()
                    << "] "
                    << p.getName()
                    << "\n";
            }
        }

        std::cout
            << "\n1. Add Position\n"
            << "2. Rename Position\n"
            << "3. Delete Position\n"
            << "4. Back\n";

        std::cout
            << "\nEnter choice: ";

        choice = takeChoice();

        switch(choice)
        {
            case 1:
                addPosition();
                break;

            case 2:
                renamePosition();
                break;

            case 3:
                deletePosition();
                break;

            case 4:
                return;

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

void ReferenceManagementUI::addPosition()
{
    clearScreen();

    std::string name;

    std::cout
        << "\nEnter Position Name: ";

    std::getline(
        std::cin,
        name
    );

    if(name.empty())
    {
        UI::error(
            "Position name cannot be empty."
        );

        pause();

        return;
    }

    try
    {
        referencesService.createPosition(
            currentSession,
            name
        );

        UI::success(
            "Position added successfully."
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

void ReferenceManagementUI::renamePosition()
{
    clearScreen();

    int positionId;

    std::cout
        << "\nEnter Position ID to rename: ";

    positionId = takeChoice();

    std::string newName;

    std::cout
        << "Enter New Position Name: ";

    std::getline(
        std::cin,
        newName
    );

    if(newName.empty())
    {
        UI::error(
            "Position name cannot be empty."
        );

        pause();

        return;
    }

    try
    {
        referencesService.updatePosition(
            currentSession,
            positionId,
            newName
        );

        UI::success(
            "Position updated successfully."
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

void ReferenceManagementUI::deletePosition()
{
    clearScreen();

    int positionId;

    std::cout
        << "\nEnter Position ID to delete: ";

    positionId = takeChoice();

    try
    {
        referencesService.deletePosition(
            currentSession,
            positionId
        );

        UI::success(
            "Position deleted successfully."
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

// =====================================================
// HELPERS
// =====================================================

int ReferenceManagementUI::takeChoice()
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

void ReferenceManagementUI::pause()
{
    std::cout
        << "\nPress Enter to continue...";

    std::cin.get();
}