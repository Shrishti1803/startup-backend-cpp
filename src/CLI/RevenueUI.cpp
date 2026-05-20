#include "CLI/RevenueUI.h"

#include <iostream>

#include "Db/Brand/BrandAggregateRepository.h"

#include "Models/Brand/Brand.h"
#include "Models/Brand/Revenue.h"
#include "CLI/BrandRenderUI.h"

RevenueUI::RevenueUI(
    BrandService& service,
    const Auth::Session& session
)
    : brandService(service),
      currentSession(session)
{
}

void RevenueUI::handle(int brandId)
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

        renderer.displayCompactHeader(data);

        renderer.displayRevenue(data);

        // =========================================
        // ACTIONS
        // =========================================

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

            renderer.displayRevenue(data);

            BrandAggregate updateData;

            Revenue r;

            std::cout
                << "\nEnter Revenue Value (required): ";

            std::getline(
                std::cin,
                input
            );

            if(
                !BaseUI::isValidDouble(
                    input
                )
            )
            {
                UI::error(
                    "Invalid revenue value."
                );

                pause();

                continue;
            }

            r.setValue(
                std::stod(input)
            );

            std::cout
                << "Year (optional): ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                if(
                    !BaseUI::isValidInteger(
                        input
                    )
                )
                {
                    UI::error(
                        "Invalid year."
                    );

                    pause();

                    continue;
                }

                r.setYear(
                    std::stoi(input)
                );
            }

            std::cout
                << "Period (optional): ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                r.setPeriod(input);
            }

            std::cout
                << "Source (optional): ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                r.setSource(input);
            }

            std::cout
                << "Type (optional): ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                r.setType(input);
            }

            std::cout
                << "Currency (optional): ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                r.setCurrency(input);
            }

            updateData.revenue
                .push_back(r);

            brandService.updateBrand(
                currentSession,
                brandId,
                updateData
            );

            UI::success(
                "Revenue added successfully."
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

            renderer.displayRevenue(data);

            BrandAggregate updateData;

            int id;

            std::cout
                << "\nEnter Revenue ID: ";

            id = takeChoice();

            Revenue r;

            r.setId(id);

            clearScreen();

            renderer.displayRevenue(data);

            std::cout
                << "\n--- Update Revenue Field ---\n";

            std::cout
                << "1. Value\n"
                << "2. Year\n"
                << "3. Period\n"
                << "4. Source\n"
                << "5. Type\n"
                << "6. Currency\n";

            int fieldChoice =
                takeChoice();

            switch(fieldChoice)
            {
                // ===== VALUE =====

                case 1:
                {
                    std::cout
                        << "New Value: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(
                        !BaseUI::isValidDouble(
                            input
                        )
                    )
                    {
                        UI::error(
                            "Invalid value."
                        );

                        pause();

                        continue;
                    }

                    r.setValue(
                        std::stod(input)
                    );

                    break;
                }

                // ===== YEAR =====

                case 2:
                {
                    std::cout
                        << "New Year: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(
                        !BaseUI::isValidInteger(
                            input
                        )
                    )
                    {
                        UI::error(
                            "Invalid year."
                        );

                        pause();

                        continue;
                    }

                    r.setYear(
                        std::stoi(input)
                    );

                    break;
                }

                // ===== PERIOD =====

                case 3:
                {
                    std::cout
                        << "New Period: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    r.setPeriod(
                        input
                    );

                    break;
                }

                // ===== SOURCE =====

                case 4:
                {
                    std::cout
                        << "New Source: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    r.setSource(
                        input
                    );

                    break;
                }

                // ===== TYPE =====

                case 5:
                {
                    std::cout
                        << "New Type: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    r.setType(
                        input
                    );

                    break;
                }

                // ===== CURRENCY =====

                case 6:
                {
                    std::cout
                        << "New Currency: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    r.setCurrency(
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

            updateData.revenue
                .push_back(r);

            brandService.updateBrand(
                currentSession,
                brandId,
                updateData
            );

            UI::success(
                "Revenue updated successfully."
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

            renderer.displayRevenue(data);

            BrandAggregate deleteData;

            int id;

            std::cout
                << "\nEnter Revenue ID to delete: ";

            id = takeChoice();

            Revenue r;

            r.setId(id);

            deleteData.revenue
                .push_back(r);

            brandService.deletePartial(
                currentSession,
                brandId,
                deleteData
            );

            UI::success(
                "Revenue deleted successfully."
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