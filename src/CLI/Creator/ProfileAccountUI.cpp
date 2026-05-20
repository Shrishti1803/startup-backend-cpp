#include "CLI/Creator/ProfileAccountUI.h"
#include "CLI/UI/TerminalLayout.h"

#include <iostream>
#include <limits>

#include "Models/Creator/ProfileAccount.h"
#include "Models/References/Platform.h"

ProfileAccountUI::ProfileAccountUI(
    CreatorService& creatorService,
    ReferencesService& referencesService,
    const Auth::Session& session
)
    : creatorService(creatorService),
      referencesService(referencesService),
      currentSession(session)
{
}

void ProfileAccountUI::handle(
    int creatorId
)
{
    int action;

    do
    {
        clearScreen();

        CreatorAggregate fullData =
            creatorService.getCreatorById(
                currentSession,
                creatorId
            );

        CreatorRenderUI renderer(
            referencesService,
            currentSession
        );

        // =========================================
        // INITIAL SCREEN
        // =========================================

        auto platforms =
            referencesService
                .getAllPlatforms(
                    currentSession
                );

        renderer.displayCompactHeader(
            fullData.creator
        );

        renderer.displayProfileAccounts(
            fullData.creator,
            platforms
        );

        // =========================================
        // AVAILABLE PLATFORMS
        // =========================================

        UI::title("AVAILABLE PLATFORMS");

        if(platforms.empty())
        {
            UI::warning(
                "No platforms available."
            );
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
            << "[0] Create New Platform\n";

        // =========================================
        // ACTIONS
        // =========================================

        UI::title("PROFILE ACCOUNT MANAGEMENT");

        std::cout
            << "1. Add\n"
            << "2. Update\n"
            << "3. Delete\n"
            << "4. Back\n";

        action = takeChoice();

        // =========================================
        // ADD
        // =========================================

        if(action == 1)
        {
            clearScreen();

            renderer.displayProfileAccounts(
                fullData.creator,
                platforms
            );

            UI::title("ADD PROFILE ACCOUNT");

            CreatorAggregate updateData;

            ProfileAccount acc;

            std::string input;

            int platformId;

            UI::title("AVAILABLE PLATFORMS");

            for(const auto& p : platforms)
            {
                std::cout
                    << "["
                    << p.getId()
                    << "] "
                    << p.getName()
                    << "\n";
            }

            std::cout
                << "[0] Create New Platform\n";

            std::cout
                << "\nEnter Platform ID: ";

            platformId = takeChoice();

            // =========================================
            // CREATE NEW PLATFORM
            // =========================================

            if(platformId == 0)
            {
                std::string newPlatform;

                std::cout
                    << "Enter New Platform Name: ";

                std::getline(
                    std::cin,
                    newPlatform
                );

                if(newPlatform.empty())
                {
                    UI::error(
                        "Platform name cannot be empty."
                    );

                    pause();

                    continue;
                }

                referencesService.createPlatform(
                    currentSession,
                    newPlatform
                );

                UI::success(
                    "Platform created successfully."
                );

                auto updatedPlatforms =
                    referencesService
                        .getAllPlatforms(
                            currentSession
                        );

                if(!updatedPlatforms.empty())
                {
                    platformId =
                        updatedPlatforms
                            .back()
                            .getId();
                }
            }

            acc.setPlatformId(
                platformId
            );

            std::cout
                << "Enter Profile Link: ";

            std::getline(
                std::cin,
                input
            );

            if(input.empty())
            {
                UI::error(
                    "Profile link cannot be empty."
                );

                pause();

                continue;
            }

            acc.setLink(
                input
            );

            std::cout
                << "Enter Followers Count "
                << "(numbers only): ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                try
                {
                    acc.setFollowers(
                        std::stoll(input)
                    );
                }
                catch(const std::exception&)
                {
                    UI::error(
                        "Followers count must be numeric."
                    );

                    pause();

                    continue;
                }
            }

            updateData.profileAccounts
                .push_back(acc);

            creatorService.updateCreator(
                currentSession,
                creatorId,
                updateData
            );

            UI::success(
                "Profile account added successfully."
            );

            pause();

            return;
        }

        // =========================================
        // UPDATE
        // =========================================

        else if(action == 2)
        {
            clearScreen();

            renderer.displayProfileAccounts(
                fullData.creator,
                platforms
            );

            UI::title("UPDATE PROFILE ACCOUNT");

            CreatorAggregate updateData;

            ProfileAccount acc;

            int id;

            UI::title("AVAILABLE PLATFORMS");

            for(const auto& p : platforms)
            {
                std::cout
                    << "["
                    << p.getId()
                    << "] "
                    << p.getName()
                    << "\n";
            }

            std::cout
                << "[0] Create New Platform\n";

            std::cout
                << "\nEnter Profile ID "
                << "(0 to back): ";

            id = takeChoice();

            if(id == 0)
            {
                return;
            }

            acc.setId(id);

            clearScreen();

            renderer.displayProfileAccounts(
                fullData.creator,
                platforms
            );

            UI::title("UPDATE PROFILE FIELD");

            std::cout
                << "1. Platform\n"
                << "2. Link\n"
                << "3. Followers\n";

            int fieldChoice =
                takeChoice();

            std::string input;

            switch(fieldChoice)
            {
                // ===== PLATFORM =====

                case 1:
                {
                    int platformId;

                    std::cout
                        << "Enter Platform ID: ";

                    platformId = takeChoice();

                    if(platformId == 0)
                    {
                        std::string newPlatform;

                        std::cout
                            << "Enter New Platform Name: ";

                        std::getline(
                            std::cin,
                            newPlatform
                        );

                        if(newPlatform.empty())
                        {
                            UI::error(
                                "Platform name cannot be empty."
                            );

                            pause();

                            continue;
                        }

                        referencesService.createPlatform(
                            currentSession,
                            newPlatform
                        );

                        UI::success(
                            "Platform created successfully."
                        );

                        auto updatedPlatforms =
                            referencesService
                                .getAllPlatforms(
                                    currentSession
                                );

                        if(!updatedPlatforms.empty())
                        {
                            platformId =
                                updatedPlatforms
                                    .back()
                                    .getId();
                        }
                    }

                    acc.setPlatformId(
                        platformId
                    );

                    break;
                }

                // ===== LINK =====

                case 2:
                {
                    std::cout
                        << "Enter New Link: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Profile link cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    acc.setLink(
                        input
                    );

                    break;
                }

                // ===== FOLLOWERS =====

                case 3:
                {
                    std::cout
                        << "Enter Followers Count "
                        << "(numbers only): ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(!input.empty())
                    {
                        try
                        {
                            acc.setFollowers(
                                std::stoll(input)
                            );
                        }
                        catch(const std::exception&)
                        {
                            UI::error(
                                "Followers count must be numeric."
                            );

                            pause();

                            continue;
                        }
                    }

                    break;
                }

                default:
                {
                    UI::error(
                        "Invalid choice."
                    );

                    pause();

                    continue;
                }
            }

            updateData.profileAccounts
                .push_back(acc);

            creatorService.updateCreator(
                currentSession,
                creatorId,
                updateData
            );

            UI::success(
                "Profile account updated successfully."
            );

            pause();

            return;
        }

        // =========================================
        // DELETE
        // =========================================

        else if(action == 3)
        {
            clearScreen();

            renderer.displayProfileAccounts(
                fullData.creator,
                platforms
            );

            UI::title("DELETE PROFILE ACCOUNT");

            CreatorAggregate deleteData;

            ProfileAccount acc;

            int id;

            std::cout
                << "Enter Profile ID to delete "
                << "(0 to back): ";

            id = takeChoice();

            if(id == 0)
            {
                return;
            }

            acc.setId(id);

            deleteData.profileAccounts
                .push_back(acc);

            creatorService.deletePartial(
                currentSession,
                creatorId,
                deleteData
            );

            UI::success(
                "Profile account deleted successfully."
            );

            pause();

            return;
        }

        else if(action == 4)
        {
            return;
        }

        else
        {
            UI::error(
                "Invalid choice."
            );

            pause();

            continue;
        }

    } while(true);
}