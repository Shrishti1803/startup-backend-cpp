#include "CLI/GoalsUI.h"

#include <limits>
#include <iostream>

#include "Db/Brand/BrandAggregateRepository.h"
#include "Models/Brand/Goal.h"
#include "CLI/BrandRenderUI.h"

GoalsUI::GoalsUI(
    BrandService& service,
    const Auth::Session& session
)
    : brandService(service),
      currentSession(session)
{
}

void GoalsUI::handle(
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

        renderer.displayGoals(
            data
        );

        std::cout
            << "\n1. Add\n"
            << "2. Update\n"
            << "3. Delete\n"
            << "4. Back\n";

        std::cout
            << "\nEnter choice: ";

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

            renderer.displayGoals(
                data
            );

            BrandAggregate updateData;

            Goal g;

            std::cout
                << "\nEnter Goal Description: ";

            std::getline(
                std::cin,
                input
            );

            if(input.empty())
            {
                UI::error(
                    "Goal description cannot be empty."
                );

                pause();

                continue;
            }

            g.setDescription(
                input
            );

            updateData.goals
                .push_back(g);

            try
            {
                brandService.updateBrand(
                    currentSession,
                    brandId,
                    updateData
                );

                UI::success(
                    "Goal added successfully."
                );
            }

            catch(const std::exception& e)
            {
                UI::error(
                    e.what()
                );
            }

            pause();

            return;
        }

        // =========================================
        // UPDATE
        // =========================================

        else if(action == 2)
        {
            clearScreen();

            renderer.displayGoals(
                data
            );

            if(data.goals.empty())
            {
                UI::warning(
                    "No goals available to update."
                );

                pause();

                continue;
            }

            BrandAggregate updateData;

            int id;

            std::cout
                << "\nEnter Goal ID "
                << "(0 to cancel): ";

            id = takeChoice();

            if(id == 0)
            {
                continue;
            }

            Goal g;

            g.setId(id);

            clearScreen();

            renderer.displayGoals(
                data
            );

            std::cout
                << "\n--- Update Goal ---\n";

            std::cout
                << "1. Description\n";

            int fieldChoice =
                takeChoice();

            switch(fieldChoice)
            {
                // ===== DESCRIPTION =====

                case 1:
                {
                    std::cout
                        << "Enter New Description: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Description cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    g.setDescription(
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

            updateData.goals
                .push_back(g);

            try
            {
                brandService.updateBrand(
                    currentSession,
                    brandId,
                    updateData
                );

                UI::success(
                    "Goal updated successfully."
                );
            }

            catch(const std::exception& e)
            {
                UI::error(
                    e.what()
                );
            }

            pause();

            return;
        }

        // =========================================
        // DELETE
        // =========================================

        else if(action == 3)
        {
            clearScreen();

            renderer.displayGoals(
                data
            );

            if(data.goals.empty())
            {
                UI::warning(
                    "No goals available to delete."
                );

                pause();

                continue;
            }

            BrandAggregate deleteData;

            int id;

            std::cout
                << "\nEnter Goal ID to delete "
                << "(0 to cancel): ";

            id = takeChoice();

            if(id == 0)
            {
                continue;
            }

            Goal g;

            g.setId(id);

            deleteData.goals
                .push_back(g);

            try
            {
                brandService.deletePartial(
                    currentSession,
                    brandId,
                    deleteData
                );

                UI::success(
                    "Goal deleted successfully."
                );
            }

            catch(const std::exception& e)
            {
                UI::error(
                    e.what()
                );
            }

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