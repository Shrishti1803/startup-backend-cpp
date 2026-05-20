#include "CLI/CompetitorsUI.h"

#include <iostream>
#include <limits>

#include "Db/Brand/BrandAggregateRepository.h"
#include "Models/Brand/Competitor.h"
#include "CLI/UI/TerminalLayout.h"

CompetitorsUI::CompetitorsUI(
    BrandService& service,
    const Auth::Session& session
)
    : brandService(service),
      currentSession(session)
{
}

void CompetitorsUI::handle(
    int brandId
)
{
    int action;

    do
    {
        clearScreen();

        BrandAggregate data =
            brandService.getBrandById(
                currentSession,
                brandId
            );

        BrandRenderer renderer;

        // =========================================
        // INITIAL SCREEN
        // =========================================

        renderer.displayCompactHeader(
            data
        );

        renderer.displayCompetitors(
            data
        );

        std::cout
            << "\n1. Add\n"
            << "2. Update\n"
            << "3. Delete\n"
            << "4. Back\n";

        action = takeChoice();

        if(action == 4)
        {
            return;
        }

        std::string input;

        // =========================================
        // ADD
        // =========================================

        if(action == 1)
        {
            clearScreen();

            renderer.displayCompetitors(
                data
            );

            BrandAggregate updateData;

            Competitor c;

            std::cout
                << "\nEnter Competitor Name: ";

            std::getline(
                std::cin,
                input
            );

            if(input.empty())
            {
                UI::error(
                    "Competitor name cannot be empty."
                );

                pause();

                continue;
            }

            c.setName(
                input
            );

            std::cout
                << "Enter Type (optional): ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                c.setType(
                    input
                );
            }

            updateData.competitors
                .push_back(c);

            brandService.updateBrand(
                currentSession,
                brandId,
                updateData
            );

            UI::success(
                "Competitor added successfully."
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

            renderer.displayCompetitors(
                data
            );

            if(data.competitors.empty())
            {
                UI::warning(
                    "No competitors available to update."
                );

                pause();

                return;
            }

            BrandAggregate updateData;

            int id;

            std::cout
                << "\nEnter Competitor ID "
                << "(0 to cancel): ";

            id = takeChoice();

            if(id == 0)
            {
                continue;
            }

            Competitor c;

            c.setId(id);

            clearScreen();

            renderer.displayCompetitors(
                data
            );

            std::cout
                << "\n--- Update Competitor Field ---\n";

            std::cout
                << "1. Name\n"
                << "2. Type\n";

            int fieldChoice =
                takeChoice();

            switch(fieldChoice)
            {
                // ===== NAME =====

                case 1:
                {
                    std::cout
                        << "Enter New Name: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Name cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    c.setName(
                        input
                    );

                    break;
                }

                // ===== TYPE =====

                case 2:
                {
                    std::cout
                        << "Enter New Type: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Type cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    c.setType(
                        input
                    );

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

            updateData.competitors
                .push_back(c);

            brandService.updateBrand(
                currentSession,
                brandId,
                updateData
            );

            UI::success(
                "Competitor updated successfully."
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

            renderer.displayCompetitors(
                data
            );

            if(data.competitors.empty())
            {
                UI::warning(
                    "No competitors available to delete."
                );

                pause();

                return;
            }

            BrandAggregate deleteData;

            int id;

            std::cout
                << "\nEnter Competitor ID to delete "
                << "(0 to cancel): ";

            id = takeChoice();

            if(id == 0)
            {
                continue;
            }

            Competitor c;

            c.setId(id);

            deleteData.competitors
                .push_back(c);

            brandService.deletePartial(
                currentSession,
                brandId,
                deleteData
            );

            UI::success(
                "Competitor deleted successfully."
            );

            pause();

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