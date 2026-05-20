// NicheUI.cpp

#include "CLI/Creator/NicheUI.h"
#include "CLI/UI/TerminalLayout.h"

#include <iostream>
#include <limits>

#include "Models/References/Niche.h"
#include "Models/Creator/CreatorNiche.h"

NicheUI::NicheUI(
    CreatorService& creatorService,
    ReferencesService& referencesService,
    const Auth::Session& session
)
    : creatorService(creatorService),
      referencesService(referencesService),
      currentSession(session)
{
}

void NicheUI::handle(
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

        renderer.displayNiches(
            fullData.creator
        );

        // =========================================
        // ALL AVAILABLE NICHES
        // =========================================

        auto allNiches =
            referencesService.getAllNiches(
                currentSession
            );

        UI::title("ALL AVAILABLE NICHES");

        if(allNiches.empty())
        {
            UI::warning(
                "No niches available."
            );
        }

        else
        {
            for(const auto& niche :
                allNiches)
            {
                std::cout
                    << "["
                    << niche.getId()
                    << "] "
                    << niche.getType()
                    << "\n";
            }
        }

        // =========================================
        // ACTIONS
        // =========================================

        UI::title("NICHE MANAGEMENT");

        std::cout
            << "1. Link Existing\n"
            << "2. Create New Niche\n"
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

            renderer.displayNiches(
                fullData.creator
            );

            UI::title("LINK EXISTING NICHE");

            for(const auto& niche :
                allNiches)
            {
                std::cout
                    << "["
                    << niche.getId()
                    << "] "
                    << niche.getType()
                    << "\n";
            }

            int id;

            std::cout
                << "\nEnter Niche ID to link "
                << "(0 to back): ";

            id = takeChoice();

            if(id == 0)
            {
                return;
            }

            CreatorAggregate updateData;

            // =========================================
            // Preserve existing niches
            // =========================================

            updateData.niches =
                fullData.niches;

            // =========================================
            // Add new niche link
            // =========================================

            Niche n;

            n.setId(id);

            CreatorNiche cn;

            cn.setNiche(n);

            updateData.niches
                .push_back(cn);

            creatorService.updateCreator(
                currentSession,
                creatorId,
                updateData
            );

            UI::success(
                "Niche linked successfully."
            );

            pause();

            return;
        }

        // =========================================
        // CREATE NEW NICHE
        // =========================================

        else if(action == 2)
        {
            clearScreen();

            renderer.displayNiches(
                fullData.creator
            );

            UI::title("CREATE NEW NICHE");

            std::string name;

            std::cout
                << "\nEnter New Niche Name: ";

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

                continue;
            }

            referencesService.createNiche(
                currentSession,
                name
            );

            UI::success(
                "Niche created successfully."
            );

            pause();

            return;
        }

        // =========================================
        // UPDATE LINKS (FULL REPLACE)
        // =========================================

        else if(action == 3)
        {
            clearScreen();

            renderer.displayNiches(
                fullData.creator
            );

            UI::title("UPDATE NICHE LINKS");

            for(const auto& niche :
                allNiches)
            {
                std::cout
                    << "["
                    << niche.getId()
                    << "] "
                    << niche.getType()
                    << "\n";
            }

            std::cout
                << "\nEnter Niche IDs "
                << "(space separated, use -1 to finish):\n";

            std::vector<CreatorNiche> newSet;

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

                Niche n;

                n.setId(id);

                CreatorNiche cn;

                cn.setNiche(n);

                newSet.push_back(cn);
            }

            std::cin.ignore(
                std::numeric_limits<
                    std::streamsize
                >::max(),
                '\n'
            );

            CreatorAggregate updateData;

            updateData.niches =
                newSet;

            creatorService.updateCreator(
                currentSession,
                creatorId,
                updateData
            );

            UI::success(
                "Niche links updated successfully."
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

            renderer.displayNiches(
                fullData.creator
            );

            UI::title("REMOVE NICHE LINK");

            int id;

            std::cout
                << "\nEnter Niche ID to remove "
                << "(0 to back): ";

            id = takeChoice();

            if(id == 0)
            {
                return;
            }

            CreatorAggregate deleteData;

            Niche n;

            n.setId(id);

            CreatorNiche cn;

            cn.setNiche(n);

            deleteData.niches
                .push_back(cn);

            creatorService.deletePartial(
                currentSession,
                creatorId,
                deleteData
            );

            UI::success(
                "Niche link removed successfully."
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
            UI::error(
                "Invalid choice."
            );

            pause();

            continue;
        }

    } while(true);
}