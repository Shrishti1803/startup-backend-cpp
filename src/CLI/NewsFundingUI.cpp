#include "CLI/NewsFundingUI.h"

#include <iostream>
#include <limits>

#include "Db/Brand/BrandAggregateRepository.h"

#include "Models/Brand/NewsFunding.h"
#include "Models/Brand/Brand.h"
#include "CLI/BrandRenderUI.h"

NewsFundingUI::NewsFundingUI(
    BrandService& service,
    const Auth::Session& session
)
    : brandService(service),
      currentSession(session)
{
}

void NewsFundingUI::handle(
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

        renderer.displayNewsFunding(
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

            renderer.displayNewsFunding(
                data
            );

            BrandAggregate updateData;

            NewsFunding n;

            std::cout
                << "\nEnter Event Type: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                n.setEventType(
                    input
                );
            }

            std::cout
                << "Enter Title: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                n.setTitle(
                    input
                );
            }

            std::cout
                << "Enter Description: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                n.setDescription(
                    input
                );
            }

            std::cout
                << "Enter Date (YYYY-MM-DD): ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                try
                {
                    Date d =
                        Date::fromString(
                            input
                        );

                    n.setDate(d);
                }

                catch(...)
                {
                    UI::error(
                        "Invalid date format."
                    );

                    pause();

                    continue;
                }
            }

            if(
                !n.getEventType().has_value() &&
                !n.getTitle().has_value() &&
                !n.getDescription().has_value() &&
                !n.getDate().has_value()
            )
            {
                UI::error(
                    "At least one field is required."
                );

                pause();

                continue;
            }

            updateData.newsFundings
                .push_back(n);

            try
            {
                brandService.updateBrand(
                    currentSession,
                    brandId,
                    updateData
                );

                UI::success(
                    "News/Funding record added successfully."
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

            renderer.displayNewsFunding(
                data
            );

            if(data.newsFundings.empty())
            {
                UI::error(
                    "No news/funding records available."
                );

                pause();

                continue;
            }

            BrandAggregate updateData;

            int id;

            std::cout
                << "\nEnter News ID: ";

            id = takeChoice();

            NewsFunding n;

            n.setId(id);

            clearScreen();

            renderer.displayNewsFunding(
                data
            );

            std::cout
                << "\n--- Update News/Funding Field ---\n";

            std::cout
                << "1. Event Type\n"
                << "2. Title\n"
                << "3. Description\n"
                << "4. Date\n";

            std::cout
                << "\nEnter choice: ";

            int fieldChoice =
                takeChoice();

            switch(fieldChoice)
            {
                // ===== EVENT TYPE =====

                case 1:
                {
                    std::cout
                        << "Enter New Event Type: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Event Type cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    n.setEventType(
                        input
                    );

                    break;
                }

                // ===== TITLE =====

                case 2:
                {
                    std::cout
                        << "Enter New Title: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Title cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    n.setTitle(
                        input
                    );

                    break;
                }

                // ===== DESCRIPTION =====

                case 3:
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

                    n.setDescription(
                        input
                    );

                    break;
                }

                // ===== DATE =====

                case 4:
                {
                    std::cout
                        << "Enter New Date (YYYY-MM-DD): ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Date cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    try
                    {
                        Date d =
                            Date::fromString(
                                input
                            );

                        n.setDate(d);
                    }

                    catch(...)
                    {
                        UI::error(
                            "Invalid date format."
                        );

                        pause();

                        continue;
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

            updateData.newsFundings
                .push_back(n);

            try
            {
                brandService.updateBrand(
                    currentSession,
                    brandId,
                    updateData
                );

                UI::success(
                    "News/Funding record updated successfully."
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

            renderer.displayNewsFunding(
                data
            );

            if(data.newsFundings.empty())
            {
                UI::error(
                    "No news/funding records available."
                );

                pause();

                continue;
            }

            BrandAggregate deleteData;

            int id;

            std::cout
                << "\nEnter News ID to delete: ";

            id = takeChoice();

            NewsFunding n;

            n.setId(id);

            deleteData.newsFundings
                .push_back(n);

            try
            {
                brandService.deletePartial(
                    currentSession,
                    brandId,
                    deleteData
                );

                UI::success(
                    "News/Funding record deleted successfully."
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