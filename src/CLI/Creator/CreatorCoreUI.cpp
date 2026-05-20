#include "CLI/Creator/CreatorCoreUI.h"

#include <iostream>
#include <limits>

#include "Models/Creator/Creator.h"

CreatorCoreUI::CreatorCoreUI(
    CreatorService& service,
    ReferencesService& referencesService,
    const Auth::Session& session
)
    : creatorService(service),
      referencesService(referencesService),
      currentSession(session)
{
}

void CreatorCoreUI::handle(int creatorId)
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

        // ===== INITIAL SCREEN =====

        renderer.displayCompactHeader(
            fullData.creator
        );

        renderer.displayCreatorCore(
            fullData.creator
        );
        UI::title("CREATOR CORE");
        std::cout
            << "1. Add / Replace\n"
            << "2. Update\n"
            << "3. Clear Field\n"
            << "4. Back\n";

        action = takeChoice();

        if(action == 4)
        {
            return;
        }

        std::string input;

        // ================= ADD / REPLACE =================

        if(action == 1)
        {
            clearScreen();

            renderer.displayCreatorCore(
                fullData.creator
            );
            UI::title("ADD / REPLACE CREATOR");
            CreatorAggregate updateData;

            Creator creator;

            UI::warning(
                "Leave blank to skip a field."
            );
            std::cout
                << "Enter Creator Name: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                creator.setName(input);
            }

            std::cout
                << "Enter Email: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                creator.setEmail(input);
            }

            std::cout
                << "Enter Rating: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                creator.setRating(input);
            }

            std::cout
                << "Enter Audience Size (numbers only): ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                try
                {
                    creator.setAudienceSize(
                        std::stoll(input)
                    );
                }
                catch(const std::exception&)
                {
                    UI::error(
                        "Audience Size must be a valid number."
                    );

                    pause();

                    return;
                }
            }

            updateData.creator = creator;

            creatorService.updateCreator(
                currentSession,
                creatorId,
                updateData
            );

            UI::success(
                "Creator updated successfully."
            );

            pause();

            return;
        }

        // ================= UPDATE =================

        else if(action == 2)
        {
            clearScreen();

            renderer.displayCreatorCore(
                fullData.creator
            );

            CreatorAggregate updateData;

            Creator creator;

            UI::title("UPDATE CREATOR FIELD");

            std::cout
                << "1. Creator Name\n"
                << "2. Email\n"
                << "3. Rating\n"
                << "4. Audience Size\n";

            int field =
                takeChoice();

            std::cout
                << "Enter New Value: ";

            std::getline(
                std::cin,
                input
            );

            switch(field)
            {
                case 1:
                {
                    creator.setName(input);
                    break;
                }

                case 2:
                {
                    creator.setEmail(input);
                    break;
                }

                case 3:
                {
                    creator.setRating(input);
                    break;
                }

                case 4:
                {
                if(!input.empty())
                {
                    try
                    {
                        creator.setAudienceSize(
                            std::stoll(input)
                        );
                    }
                    catch(const std::exception&)
                    {
                        UI::error(
                            "Audience Size must be a valid number."
                        );

                        pause();

                        continue;
                    }
                }

                    break;
                }

                default:
                {
                    UI::error("Invalid choice.");
                    pause();

                    continue;
                }
            }

            updateData.creator = creator;

            creatorService.updateCreator(
                currentSession,
                creatorId,
                updateData
            );

            std::cout
                << "\nCreator updated successfully.\n";

            pause();

            return;
        }

        // ================= CLEAR FIELD =================

        else if(action == 3)
        {
            clearScreen();

            renderer.displayCreatorCore(
                fullData.creator
            );

            CreatorAggregate updateData;

            Creator creator;

            UI::title("CLEAR CREATOR FIELD");

            std::cout
                << "1. Email\n"
                << "2. Rating\n"
                << "3. Audience Size\n";

            int field =
                takeChoice();

            switch(field)
            {
                case 1:
                {
                    creator.setEmail(
                        std::nullopt
                    );

                    updateData.clearFields.insert(
                        "creator.email"
                    );

                    break;
                }

                case 2:
                {
                    creator.setRating(
                        std::nullopt
                    );

                    updateData.clearFields.insert(
                        "creator.rating"
                    );

                    break;
                }

                case 3:
                {
                    creator.setAudienceSize(
                        std::nullopt
                    );

                    updateData.clearFields.insert(
                        "creator.audience_size"
                    );

                    break;
                }

                default:
                {
                    std::cout
                        << "\nInvalid choice.\n";

                    pause();

                    continue;
                }
            }

            updateData.creator = creator;

            creatorService.updateCreator(
                currentSession,
                creatorId,
                updateData
            );

            std::cout
                << "\nCreator updated successfully.\n";

            pause();

            return;
        }

        else
        {
            std::cout
                << "\nInvalid choice.\n";

            pause();

            continue;
        }

    } while(true);
}