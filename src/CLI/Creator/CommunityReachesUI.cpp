#include "CLI/Creator/CommunityReachesUI.h"

#include <iostream>
#include <limits>

#include "Models/Creator/CommunityReaches.h"
CommunityReachesUI::CommunityReachesUI(
    CreatorService& service,
    ReferencesService& referenceService,
    const Auth::Session& session
)
    : creatorService(service),
    referencesService(referenceService),
    currentSession(session)
{
}

void CommunityReachesUI::handle(
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

        // ===== INITIAL SCREEN =====

        renderer.displayCompactHeader(
            fullData.creator
        );

        renderer.displayCommunity(
            fullData.creator
        );

        UI::title("COMMUNITY REACHES");
        std::cout
            << "1. Add / Replace\n"
            << "2. Update Single Field\n"
            << "3. Clear Field\n"
            << "4. Back\n";

        action = takeChoice();

        if(action == 4)
        {
            return;
        }

        std::string input;

        // =====================================================
        // ADD / REPLACE
        // =====================================================

        if(action == 1)
        {
            clearScreen();

            UI::title("ADD / REPLACE COMMUNITY");
            renderer.displayCommunity(
                fullData.creator
            );

            CreatorAggregate updateData;

            CommunityReaches c;

            std::cout
                << "Enter Sentiment: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                c.setSentiment(
                    input
                );
            }

            std::cout
                << "Enter Reply Rate: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                c.setReplyRate(
                    input
                );
            }

            updateData.community = c;

            creatorService.updateCreator(
                currentSession,
                creatorId,
                updateData
            );

            UI::success(
                "Community reaches updated successfully."
            );

            pause();

            return;
        }

        // =====================================================
        // UPDATE SINGLE FIELD
        // =====================================================

        else if(action == 2)
        {
            clearScreen();

            renderer.displayCommunity(
                fullData.creator
            );

            CreatorAggregate updateData;

            CommunityReaches c;

            UI::title("UPDATE COMMUNITY FIELD");

            std::cout
                << "1. Sentiment\n"
                << "2. Reply Rate\n";

            int field =
                takeChoice();

            std::cout
                << "Enter New Value: ";

            std::getline(
                std::cin,
                input
            );

            if(field == 1)
            {
                c.setSentiment(
                    input
                );
            }

            else if(field == 2)
            {
                c.setReplyRate(
                    input
                );
            }

            else
            {
                UI::error("Invalid choice.");

                pause();

                continue;
            }

            updateData.community = c;

            creatorService.updateCreator(
                currentSession,
                creatorId,
                updateData
            );

            UI::success(
                "Community reaches updated successfully."
            );
            pause();

            return;
        }

        // =====================================================
        // CLEAR FIELD
        // =====================================================

        else if(action == 3)
        {
            clearScreen();

            renderer.displayCommunity(
                fullData.creator
            );

            CreatorAggregate updateData;

            CommunityReaches c;

            UI::title("CLEAR COMMUNITY FIELD");

            std::cout
                << "1. Sentiment\n"
                << "2. Reply Rate\n";

            int field =
                takeChoice();

            if(field == 1)
            {
                c.setSentiment(
                    std::nullopt
                );

                updateData.clearFields.insert(
                    "community_reaches.sentiment"
                );
            }

            else if(field == 2)
            {
                c.setReplyRate(
                    std::nullopt
                );

                updateData.clearFields.insert(
                    "community_reaches.reply_rate"
                );
            }

            else
            {
                UI::error("Invalid choice.");

                pause();

                continue;
            }

            updateData.community = c;

            creatorService.updateCreator(
                currentSession,
                creatorId,
                updateData
            );

            UI::success(
                "Community reaches updated successfully."
            );
            pause();

            return;
        }

        else
        {
            UI::error("Invalid choice.");

            pause();

            continue;
        }

    } while(true);
}