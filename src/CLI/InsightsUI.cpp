#include "CLI/InsightsUI.h"

#include <iostream>
#include <limits>

#include "Models/Brand/Insights.h"
#include "Models/Brand/Brand.h"
#include "CLI/BrandRenderUI.h"

InsightsUI::InsightsUI(
    BrandService& service,
    const Auth::Session& session
)
    : brandService(service),
      currentSession(session)
{
}

void InsightsUI::handle(
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

        renderer.displayInsights(
            data
        );

        std::cout
            << "\n1. Add / Replace\n"
            << "2. Update Single Field\n"
            << "3. Clear Field\n"
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
        // ADD / REPLACE
        // =========================================

        if(action == 1)
        {
            clearScreen();

            renderer.displayInsights(
                data
            );

            BrandAggregate updateData;

            Insights ins;

            std::cout
                << "\nEnter Pitch Angle: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                ins.setPitchAngle(
                    input
                );
            }

            std::cout
                << "Enter Leverage: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                ins.setLeverage(
                    input
                );
            }

            std::cout
                << "Enter Gap: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                ins.setGap(
                    input
                );
            }

            std::cout
                << "Enter Hook: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                ins.setHook(
                    input
                );
            }

            if(
                !ins.getPitchAngle().has_value() &&
                !ins.getLeverage().has_value() &&
                !ins.getGap().has_value() &&
                !ins.getHook().has_value()
            )
            {
                UI::error(
                    "At least one field is required."
                );

                pause();

                continue;
            }

            updateData.insights =
                ins;

            try
            {
                brandService.updateBrand(
                    currentSession,
                    brandId,
                    updateData
                );

                UI::success(
                    "Insights updated successfully."
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
        // UPDATE SINGLE FIELD
        // =========================================

        else if(action == 2)
        {
            clearScreen();

            renderer.displayInsights(
                data
            );

            BrandAggregate updateData;

            Insights ins;

            std::cout
                << "\nChoose Field:\n"
                << "1. Pitch Angle\n"
                << "2. Leverage\n"
                << "3. Gap\n"
                << "4. Hook\n";

            int field =
                takeChoice();

            switch(field)
            {
                case 1:
                {
                    std::cout
                        << "Enter New Pitch Angle: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Pitch Angle cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    ins.setPitchAngle(
                        input
                    );

                    break;
                }

                case 2:
                {
                    std::cout
                        << "Enter New Leverage: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Leverage cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    ins.setLeverage(
                        input
                    );

                    break;
                }

                case 3:
                {
                    std::cout
                        << "Enter New Gap: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Gap cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    ins.setGap(
                        input
                    );

                    break;
                }

                case 4:
                {
                    std::cout
                        << "Enter New Hook: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Hook cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    ins.setHook(
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

            updateData.insights =
                ins;

            try
            {
                brandService.updateBrand(
                    currentSession,
                    brandId,
                    updateData
                );

                UI::success(
                    "Insight field updated successfully."
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
        // CLEAR FIELD
        // =========================================

        else if(action == 3)
        {
            clearScreen();

            renderer.displayInsights(
                data
            );

            BrandAggregate updateData;

            Insights ins;

            std::cout
                << "\nChoose Field To Clear:\n"
                << "1. Pitch Angle\n"
                << "2. Leverage\n"
                << "3. Gap\n"
                << "4. Hook\n";

            int field =
                takeChoice();

            switch(field)
            {
                case 1:
                {
                    ins.setPitchAngle(
                        std::nullopt
                    );

                    updateData.clearFields.insert(
                        "insights.pitch_angle"
                    );

                    break;
                }

                case 2:
                {
                    ins.setLeverage(
                        std::nullopt
                    );

                    updateData.clearFields.insert(
                        "insights.leverage"
                    );

                    break;
                }

                case 3:
                {
                    ins.setGap(
                        std::nullopt
                    );

                    updateData.clearFields.insert(
                        "insights.gap"
                    );

                    break;
                }

                case 4:
                {
                    ins.setHook(
                        std::nullopt
                    );

                    updateData.clearFields.insert(
                        "insights.hook"
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

            updateData.insights =
                ins;

            try
            {
                brandService.updateBrand(
                    currentSession,
                    brandId,
                    updateData
                );

                UI::success(
                    "Insight field cleared successfully."
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