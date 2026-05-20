#include "CLI/Creator/CreatorAudienceUI.h"

#include <limits>
#include <iostream>

#include "Models/References/TargetAudience.h"
#include "Db/Creator/CreatorAggregateRepository.h"

CreatorAudienceUI::CreatorAudienceUI(
    CreatorService& service,
    ReferencesService& refService,
    const Auth::Session& session
)
    : creatorService(service),
      referencesService(refService),
      currentSession(session)
{
}

void CreatorAudienceUI::handle(
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

        renderer.displayCompactHeader(
            fullData.creator
        );

        renderer.displayTargetAudience(
            fullData.creator
        );

        // =========================================
        // ALL AVAILABLE TARGET AUDIENCE
        // =========================================

        auto allAudience =
            referencesService.getAllTargetAudience(
                currentSession
            );

        UI::title("ALL AVAILABLE TARGET AUDIENCE");

        if(allAudience.empty())
        {
            UI::warning(
                "No target audience available."
            );
        }

        else
        {
            for(const auto& audience : allAudience)
            {
                std::cout
                    << "["
                    << audience.getId()
                    << "] "
                    << audience.getName()
                    << "\n";
            }
        }

        // =========================================
        // ACTIONS
        // =========================================

        std::cout << "\n";

        UI::title("TARGET AUDIENCE MANAGEMENT");
        std::cout
            << "1. Link Existing\n"
            << "2. Create New Audience\n"
            << "3. Update Links (Replace Full Set)\n"
            << "4. Remove Link\n"
            << "5. Back\n";

        action = takeChoice();

        // =========================================
        // LINK EXISTING
        // =========================================

        if(action == 1)
        {
            clearScreen();

            renderer.displayTargetAudience(
                fullData.creator
            );

            std::cout
                << "\n--- All Available Target Audience ---\n";

            for(const auto& audience : allAudience)
            {
                std::cout
                    << "["
                    << audience.getId()
                    << "] "
                    << audience.getName()
                    << "\n";
            }

            int id;

            std::cout
                << "\nEnter Audience ID to link: ";

            id = takeChoice();

            CreatorAggregate updateData;

            // Preserve existing audience

            updateData.targetAudience =
                fullData.targetAudience;

            // Add new audience

            TargetAudience a;

            a.setId(id);

            updateData.targetAudience
                .push_back(a);

            creatorService.updateCreator(
                currentSession,
                creatorId,
                updateData
            );

            UI::success("Audience linked.");

            pause();

            return;
        }

        // =========================================
        // CREATE NEW AUDIENCE
        // =========================================

        else if(action == 2)
        {
            clearScreen();

            renderer.displayTargetAudience(
                fullData.creator
            );

            std::string name;

            std::cout
                << "\nEnter New Audience Name: ";

            std::getline(
                std::cin,
                name
            );

            referencesService.createTargetAudience(
                currentSession,
                name
            );

            UI::success("Audience created.");

            pause();

            return;
        }

        // =========================================
        // UPDATE LINKS (FULL REPLACE)
        // =========================================

        else if(action == 3)
        {
            clearScreen();

            renderer.displayTargetAudience(
                fullData.creator
            );

            std::cout
                << "\n--- All Available Target Audience ---\n";

            for(const auto& audience : allAudience)
            {
                std::cout
                    << "["
                    << audience.getId()
                    << "] "
                    << audience.getName()
                    << "\n";
            }

            std::cout
                << "\nEnter Audience IDs "
                << "(space separated, use with -1 to finish):\n";

            std::vector<TargetAudience> newSet;

            while(true)
            {
                int id;

                if(!(std::cin >> id))
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<
                            std::streamsize
                        >::max(),
                        '\n'
                    );

                    UI::error(
                        "Enter valid numeric IDs."
                    );

                    pause();

                    return;
                }

                if(id == -1)
                {
                    break;
                }

                TargetAudience a;

                a.setId(id);

                newSet.push_back(a);
            }

            std::cin.ignore(
                std::numeric_limits<
                    std::streamsize
                >::max(),
                '\n'
            );

            CreatorAggregate updateData;

            updateData.targetAudience =
                newSet;

            creatorService.updateCreator(
                currentSession,
                creatorId,
                updateData
            );

            UI::success(
                "Audience links updated."
            );

            pause();

            return;
        }

        // =========================================
        // REMOVE LINK
        // =========================================

        else if(action == 4)
        {
            clearScreen();

            renderer.displayTargetAudience(
                fullData.creator
            );

            int id;

            std::cout
                << "\nEnter Audience ID to remove: ";

            id = takeChoice();

            CreatorAggregate deleteData;

            TargetAudience a;

            a.setId(id);

            deleteData.targetAudience
                .push_back(a);

            creatorService.deletePartial(
                currentSession,
                creatorId,
                deleteData
            );

            UI::success(
                "Audience link removed."
            );

            pause();

            return;
        }

        else if(action == 5)
        {
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