#include "CLI/GenresUI.h"

#include <iostream>
#include <limits>

#include "Models/References/Genre.h"
#include "Models/Brand/Brand.h"

GenresUI::GenresUI(
    BrandService& service,
    ReferencesService& refService,
    const Auth::Session& session
)
    : brandService(service),
      referencesService(refService),
      currentSession(session)
{
}

void GenresUI::handle(
    int brandId
)
{
    int action;

    do
    {
        clearScreen();

        std::cout
            << "\n=================================\n"
            << "           BRAND GENRES\n"
            << "=================================\n";

        // =========================================
        // CURRENT BRAND GENRES
        // =========================================

        auto brand =
            brandService.getBrandById(
                currentSession,
                brandId
            );

        std::cout
            << "\n--- Current Brand Genres ---\n";

        if(brand.genres.empty())
        {
            std::cout
                << "No linked genres.\n";
        }

        else
        {
            for(const auto& genre :
                brand.genres)
            {
                std::cout
                    << "["
                    << genre.getId()
                    << "] "
                    << genre.getCategory()
                    << "\n";
            }
        }

        // =========================================
        // ALL AVAILABLE GENRES
        // =========================================

        auto allGenres =
            referencesService.getAllGenres(
                currentSession
            );

        std::cout
            << "\n--- All Available Genres ---\n";

        if(allGenres.empty())
        {
            std::cout
                << "No genres available.\n";
        }

        else
        {
            for(const auto& genre :
                allGenres)
            {
                std::cout
                    << "["
                    << genre.getId()
                    << "] "
                    << genre.getCategory()
                    << "\n";
            }
        }

        // =========================================
        // ACTIONS
        // =========================================

        std::cout
            << "\n1. Link Existing\n"
            << "2. Create New Genre\n"
            << "3. Update Links (Replace Full Set)\n"
            << "4. Remove Link\n"
            << "5. Back\n";

        std::cout
            << "\nEnter choice: ";

        action = takeChoice();

        // =========================================
        // LINK EXISTING
        // =========================================

        if(action == 1)
        {
            clearScreen();

            std::cout
                << "\n--- Link Genre ---\n";

            for(const auto& genre :
                allGenres)
            {
                std::cout
                    << "["
                    << genre.getId()
                    << "] "
                    << genre.getCategory()
                    << "\n";
            }

            std::cout
                << "\nEnter Genre ID to link (0 to cancel): ";

            int id =
                takeChoice();

            if(id == 0)
            {
                continue;
            }

            BrandAggregate data;

            // Preserve existing genres

            data.genres =
                brand.genres;

            // Add new genre

            Genre g;

            g.setId(id);

            data.genres.push_back(g);

            try
            {
                brandService.updateBrand(
                    currentSession,
                    brandId,
                    data
                );

                UI::success(
                    "Genre linked successfully."
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

        // =========================================
        // CREATE NEW GENRE
        // =========================================

        else if(action == 2)
        {
            clearScreen();

            std::string name;

            std::cout
                << "\nEnter New Genre Name: ";

            std::getline(
                std::cin,
                name
            );

            if(name.empty())
            {
                UI::error(
                    "Genre name cannot be empty."
                );

                pause();

                continue;
            }

            try
            {
                referencesService.createGenre(
                    currentSession,
                    name
                );

                UI::success(
                    "Genre created successfully."
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

        // =========================================
        // UPDATE LINKS (FULL REPLACE)
        // =========================================

        else if(action == 3)
        {
            clearScreen();

            std::cout
                << "\n--- Replace Genre Links ---\n";

            for(const auto& genre :
                allGenres)
            {
                std::cout
                    << "["
                    << genre.getId()
                    << "] "
                    << genre.getCategory()
                    << "\n";
            }

            std::cout
                << "\nEnter Genre IDs "
                << "(space separated, end with -1):\n";

            std::vector<Genre> newSet;

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
                        "Invalid input."
                    );

                    newSet.clear();

                    break;
                }

                if(id == -1)
                {
                    break;
                }

                Genre g;

                g.setId(id);

                newSet.push_back(g);
            }

            std::cin.ignore(
                std::numeric_limits<
                    std::streamsize
                >::max(),
                '\n'
            );

            if(newSet.empty())
            {
                UI::error(
                    "No valid genre IDs entered."
                );

                pause();

                continue;
            }

            BrandAggregate data;

            data.genres =
                newSet;

            try
            {
                brandService.updateBrand(
                    currentSession,
                    brandId,
                    data
                );

                UI::success(
                    "Genre links updated successfully."
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

        // =========================================
        // REMOVE LINK
        // =========================================

        else if(action == 4)
        {
            clearScreen();

            std::cout
                << "\n--- Current Genres ---\n";

            for(const auto& genre :
                brand.genres)
            {
                std::cout
                    << "["
                    << genre.getId()
                    << "] "
                    << genre.getCategory()
                    << "\n";
            }

            std::cout
                << "\nEnter Genre ID to remove (0 to cancel): ";

            int id =
                takeChoice();

            if(id == 0)
            {
                continue;
            }

            BrandAggregate data;

            Genre g;

            g.setId(id);

            data.genres.push_back(g);

            try
            {
                brandService.deletePartial(
                    currentSession,
                    brandId,
                    data
                );

                UI::success(
                    "Genre link removed successfully."
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
        }

    } while(true);
}