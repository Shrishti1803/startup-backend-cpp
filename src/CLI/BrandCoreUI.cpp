#include "CLI/BrandCoreUI.h"
#include "CLI/UI/TerminalLayout.h"

#include <iostream>
#include <limits>

#include "Models/Brand/Brand.h"
#include "Db/Brand/BrandAggregateRepository.h"

BrandCoreUI::BrandCoreUI(
    BrandService& service,
    const Auth::Session& session
)
    : brandService(service),
      currentSession(session)
{
}

void BrandCoreUI::handle(int brandId)
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

        // ===== INITIAL SCREEN =====

        renderer.displayCompactHeader(
            data
        );

        renderer.displayBrandCore(
            data
        );

        UI::title(
            "BRAND CORE MANAGEMENT"
        );

        std::cout
            << "1. Add / Replace\n"
            << "2. Update\n"
            << "3. Clear Field\n"
            << "4. Back\n";

        action = takeChoice();

        if(action == 4)
        {
            return;
        }

        std::string input;

        // ================= ADD / REPLACE =================

        if(action == 1)
        {
            clearScreen();

            renderer.displayBrandCore(
                data
            );

            UI::title(
                "ADD / REPLACE BRAND CORE"
            );

            UI::warning(
                "Leave optional fields blank to skip."
            );

            BrandAggregate updateData;

            Brand b;

            std::cout
                << "Enter Name: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                b.setName(
                    input
                );
            }

            std::cout
                << "Enter Company Type: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                b.setCompanyType(
                    input
                );
            }

            std::cout
                << "Enter Psychographics: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                b.setPsychographics(
                    input
                );
            }

            std::cout
                << "Enter Genre Description: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                b.setGenreDescription(
                    input
                );
            }

            updateData.brand = b;

            brandService.updateBrand(
                currentSession,
                brandId,
                updateData
            );

            UI::success(
                "Brand core updated successfully."
            );

            pause();

            return;
        }

        // ================= UPDATE =================

        else if(action == 2)
        {
            clearScreen();

            renderer.displayBrandCore(
                data
            );

            UI::title(
                "UPDATE BRAND FIELD"
            );

            BrandAggregate updateData;

            Brand b;

            std::cout
                << "1. Brand Name\n"
                << "2. Company Type\n"
                << "3. Psychographics\n"
                << "4. Genre Description\n";

            int field =
                takeChoice();

            std::cout
                << "Enter New Value: ";

            std::getline(
                std::cin,
                input
            );

            if(input.empty())
            {
                UI::error(
                    "Field value cannot be empty."
                );

                pause();

                continue;
            }

            if(field == 1)
            {
                b.setName(
                    input
                );
            }

            else if(field == 2)
            {
                b.setCompanyType(
                    input
                );
            }

            else if(field == 3)
            {
                b.setPsychographics(
                    input
                );
            }

            else if(field == 4)
            {
                b.setGenreDescription(
                    input
                );
            }

            else
            {
                UI::error(
                    "Invalid choice."
                );

                pause();

                continue;
            }

            updateData.brand = b;

            brandService.updateBrand(
                currentSession,
                brandId,
                updateData
            );

            UI::success(
                "Brand updated successfully."
            );

            pause();

            return;
        }

        // ================= CLEAR FIELD =================

        else if(action == 3)
        {
            clearScreen();

            renderer.displayBrandCore(
                data
            );

            UI::title(
                "CLEAR BRAND FIELD"
            );

            BrandAggregate updateData;

            Brand b;

            std::cout
                << "1. Company Type\n"
                << "2. Psychographics\n"
                << "3. Genre Description\n";

            int field =
                takeChoice();

            if(field == 1)
            {
                b.setCompanyType(
                    std::nullopt
                );

                updateData.clearFields.insert(
                    "brand.company_type"
                );
            }

            else if(field == 2)
            {
                b.setPsychographics(
                    std::nullopt
                );

                updateData.clearFields.insert(
                    "brand.psychographics"
                );
            }

            else if(field == 3)
            {
                b.setGenreDescription(
                    std::nullopt
                );

                updateData.clearFields.insert(
                    "brand.genre_description"
                );
            }

            else
            {
                UI::error(
                    "Invalid choice."
                );

                pause();

                continue;
            }

            updateData.brand = b;

            brandService.updateBrand(
                currentSession,
                brandId,
                updateData
            );

            UI::success(
                "Field cleared successfully."
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