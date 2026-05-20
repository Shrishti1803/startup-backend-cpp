#include "CLI/Creator/CreatorUI.h"
#include "CLI/UI/TerminalLayout.h"

#include <iostream>

#include "Models/Creator/Creator.h"

CreatorUI::CreatorUI(
    CreatorService& creatorService,
    ReferencesService& referenceService,
    const Auth::Session& session
)
: creatorService(creatorService),
  referenceService(referenceService),
  currentSession(session),
  creatorRenderer(
        referenceService,
        session
    ),
  profileAccountUI(
        creatorService,
        referenceService,
        session
    )
{
}

void CreatorUI::openCreatorById(int creatorId)
{
    creatorMenu(creatorId);
}

void CreatorUI::run()
{
    int choice;

    do
    {
        clearScreen();

        UI::title("CREATORS");

        std::cout
            << "1. Open Creator\n"
            << "2. Create Creator\n"
            << "3. Delete Creator\n"
            << "4. Back\n";

        choice = takeChoice();

        switch(choice)
        {
            case 1:
                openCreator();
                break;

            case 2:
                createCreator();
                break;

            case 3:
                deleteCreator();
                break;

            case 4:
                return;

            default:
                UI::error("Invalid choice.");
                pause();
        }

    } while(true);
}


void CreatorUI::openCreator()
{
    clearScreen();

    std::vector<Creator> creators =
        creatorService.getAllCreators(
            currentSession
        );

    UI::title("CREATOR LIST");

    if(creators.empty())
    {
        UI::warning("No creators found.");

        pause();

        return;
    }

    for(const auto& creator : creators)
    {
        std::cout
            << "[" << creator.getId() << "] "
            << creator.getName()
            << "\n";
    }

    std::cout
        << "\nEnter Creator ID (0 to back): ";

    int creatorId = takeChoice();

    if(creatorId == 0)
    {
        return;
    }

    creatorMenu(creatorId);
}



void CreatorUI::createCreator()
{
    clearScreen();

    CreatorAggregate data;

    Creator creator;

    std::string input;

    UI::title("CREATE CREATOR");

    UI::warning(
        "Leave optional fields blank to skip."
    );

    std::cout << "Creator Name: ";

    std::getline(std::cin, input);

    if(input.empty())
    {
        UI::error(
            "Creator Name cannot be empty."
        );

        pause();

        return;
    }

    creator.setName(input);

    std::cout << "Email: ";

    std::getline(std::cin, input);

    if(!input.empty())
    {
        creator.setEmail(input);
    }

    std::cout << "Rating: ";

    std::getline(std::cin, input);

    if(!input.empty())
    {
        creator.setRating(input);
    }

    std::cout
        << "Audience Size (numbers only): ";

    std::getline(std::cin, input);

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

    data.creator = creator;

    int creatorId =
        creatorService.createCreator(
            currentSession,
            data
        );

    UI::success(
        "Creator created successfully."
    );

    std::cout
        << "\nOpening Creator Details...\n";

    pause();

    creatorMenu(creatorId);

    return;
}

void CreatorUI::deleteCreator()
{
    clearScreen();

    int creatorId;

    UI::title("DELETE CREATOR");

    std::cout
        << "Enter Creator ID to delete "
        << "(0 to back): ";

    creatorId = takeChoice();

    if(creatorId == 0)
    {
        return;
    }

    creatorService.deleteCreator(
        currentSession,
        creatorId
    );

    UI::success(
        "Creator deleted successfully."
    );

    pause();
}

void CreatorUI::creatorMenu(int creatorId)
{
    int choice;

    do
    {
        clearScreen();

        CreatorAggregate data =
            creatorService.getCreatorById(
                currentSession,
                creatorId
            );

        creatorRenderer.render(
            data.creator
        );

        UI::title("CREATOR MANAGEMENT");

        std::cout
            << "1. Core Fields\n"
            << "2. Community Reaches\n"
            << "3. Platform Accounts\n"
            << "4. HPV Audit\n"
            << "5. Strategic Notes\n"
            << "6. Video L10\n"
            << "7. Verdict\n"
            << "8. Niches\n"
            << "9. Target Audience\n"
            << "10. Back\n";

        choice = takeChoice();

        switch(choice)
        {
            case 1:
            {
                CreatorCoreUI ui(
                    creatorService,
                    referenceService,
                    currentSession
                );

                ui.handle(creatorId);

                break;
            }

            case 2:
            {
                CommunityReachesUI ui(
                    creatorService,
                    referenceService,
                    currentSession
                );

                ui.handle(creatorId);

                break;
            }

            case 3:
            {
                ProfileAccountUI ui(
                    creatorService,
                    referenceService,
                    currentSession
                );

                ui.handle(creatorId);

                break;
            }

            case 4:
            {
                HPVAuditUI ui(
                    creatorService,
                    referenceService,
                    currentSession
                );

                ui.handle(creatorId);

                break;
            }

            case 5:
            {
                StrategicNoteUI ui(
                    creatorService,
                    referenceService,
                    currentSession
                );

                ui.handle(creatorId);

                break;
            }

            case 6:
            {
                VideoL10UI ui(
                    creatorService,
                    referenceService,
                    currentSession
                );

                ui.handle(creatorId);

                break;
            }

            case 7:
            {
                VerdictUI ui(
                    creatorService,
                    referenceService,
                    currentSession
                );

                ui.handle(creatorId);

                break;
            }

            case 8:
            {
                NicheUI ui(
                    creatorService,
                    referenceService,
                    currentSession
                );

                ui.handle(creatorId);

                break;
            }

            case 9:
            {
                CreatorAudienceUI ui(
                    creatorService,
                    referenceService,
                    currentSession
                );

                ui.handle(creatorId);

                break;
            }

            case 10:
                return;

            default:
                UI::error("Invalid choice.");
                pause();
        }

    } while(true);
}