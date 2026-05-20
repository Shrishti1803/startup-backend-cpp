#include "CLI/Query/SearchFilterUI.h"
#include "CLI/UI/TerminalLayout.h"

#include <iostream>
#include <sstream>
#include <algorithm>

#include "CLI/Query/QueryResultsUI.h"

SearchFilterUI::SearchFilterUI(
    BrandService& brandService,
    CreatorService& creatorService,
    ReferencesService& referenceService,
    BrandUI& brandUI,
    CreatorUI& creatorUI,
    const Auth::Session& session
)
    : brandService(brandService),
      creatorService(creatorService),
      referenceService(referenceService),
      brandUI(brandUI),
      creatorUI(creatorUI),
      currentSession(session)
{
}

void SearchFilterUI::run()
{
    showMenu();
}

static std::vector<int> parseIds(
    std::string input
)
{
    std::vector<int> ids;

    // Replace commas with spaces

    std::replace(
        input.begin(),
        input.end(),
        ',',
        ' '
    );

    std::stringstream ss(input);

    int id;

    while(ss >> id)
    {
        ids.push_back(id);
    }

    return ids;
}

void SearchFilterUI::showMenu()
{
    int choice;

    do
    {
        clearScreen();

        UI::title("SEARCH & FILTER");

        std::cout
            << "1. Search\n"
            << "2. Apply Filters\n"
            << "3. Back\n";

        std::cout
            << "\nEnter choice: ";

        choice = takeChoice();

        switch(choice)
        {
            case 1:
                search();
                break;

            case 2:
                applyFilters();
                break;

            case 3:
                return;

            default:
            {
                UI::error(
                    "Invalid choice."
                );

                pause();
            }
        }

    } while(true);
}

void SearchFilterUI::search()
{
    clearScreen();

    UI::title("SEARCH");

    UI::warning(
        "Search by Brand Name, Creator Name, Genre, Niche, or Target Audience."
    );

    std::string query;

    std::cout
        << "\nSearch Query: ";

    std::getline(
        std::cin,
        query
    );

    if(query.empty())
    {
        UI::error(
            "Search query cannot be empty."
        );

        pause();

        return;
    }

    try
    {
        BrandPage brandPage =
            brandService.getBrandsView(
                currentSession,
                query
            );

        CreatorPage creatorPage =
            creatorService.getCreatorsView(
                currentSession,
                query
            );

        QueryResultsUI ui(
            brandPage,
            creatorPage,
            brandUI,
            creatorUI
        );

        ui.run();
    }

    catch(const std::exception& e)
    {
        UI::error(
            e.what()
        );

        pause();
    }
}

void SearchFilterUI::applyFilters()
{
    clearScreen();

    try
    {
        std::vector<int> genreIds;

        std::vector<int> nicheIds;

        int minAudience = -1;

        int maxAudience = -1;

        // ================= GENRES =================

        auto genres =
            referenceService.getAllGenres(
                currentSession
            );

        UI::title("AVAILABLE GENRES");

        if(genres.empty())
        {
            UI::warning(
                "No genres available."
            );
        }

        else
        {
            for(const auto& genre :
                genres)
            {
                std::cout
                    << "["
                    << genre.getId()
                    << "] "
                    << genre.getCategory()
                    << "\n";
            }
        }

        // ================= NICHES =================

        auto niches =
            referenceService.getAllNiches(
                currentSession
            );

        UI::title("AVAILABLE NICHES");

        if(niches.empty())
        {
            UI::warning(
                "No niches available."
            );
        }

        else
        {
            for(const auto& niche :
                niches)
            {
                std::cout
                    << "["
                    << niche.getId()
                    << "] "
                    << niche.getType()
                    << "\n";
            }
        }

        // ================= GENRE INPUT =================

        std::string genreInput;

        std::cout
            << "\nEnter Genre IDs "
            << "(comma/space separated, ENTER to skip): ";

        std::getline(
            std::cin,
            genreInput
        );

        genreIds = parseIds(
            genreInput
        );

        // ================= NICHE INPUT =================

        std::string nicheInput;

        std::cout
            << "\nEnter Niche IDs "
            << "(comma/space separated, ENTER to skip): ";

        std::getline(
            std::cin,
            nicheInput
        );

        nicheIds = parseIds(
            nicheInput
        );

        // ================= AUDIENCE =================

        std::string input;

        std::cout
            << "\nMinimum Audience Size "
            << "(-1 to skip): ";

        std::getline(
            std::cin,
            input
        );

        if(!input.empty())
        {
            try
            {
                minAudience =
                    std::stoi(input);
            }

            catch(const std::exception&)
            {
                UI::error(
                    "Minimum Audience Size must be numeric."
                );

                pause();

                return;
            }
        }

        std::cout
            << "Maximum Audience Size "
            << "(-1 to skip): ";

        std::getline(
            std::cin,
            input
        );

        if(!input.empty())
        {
            try
            {
                maxAudience =
                    std::stoi(input);
            }

            catch(const std::exception&)
            {
                UI::error(
                    "Maximum Audience Size must be numeric."
                );

                pause();

                return;
            }
        }

        // ================= VALIDATION =================

        if(
            minAudience != -1 &&
            maxAudience != -1 &&
            minAudience > maxAudience
        )
        {
            UI::error(
                "Minimum Audience cannot exceed Maximum Audience."
            );

            pause();

            return;
        }

        // ================= FETCH RESULTS =================

        BrandPage brandPage =
            brandService.getBrandsView(
                currentSession,
                "",
                genreIds
            );

        CreatorPage creatorPage =
            creatorService.getCreatorsView(
                currentSession,
                "",
                minAudience,
                maxAudience,
                nicheIds
            );

        QueryResultsUI ui(
            brandPage,
            creatorPage,
            brandUI,
            creatorUI
        );

        ui.run();
    }

    catch(const std::exception& e)
    {
        UI::error(
            e.what()
        );

        pause();
    }
}