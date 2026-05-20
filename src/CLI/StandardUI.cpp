#include "CLI/StandardUI.h"

#include <iostream>
#include <limits>

#include "Models/Brand/Standard.h"
#include "Models/Brand/Brand.h"
#include "CLI/BrandRenderUI.h"

StandardUI::StandardUI(
    BrandService& service,
    const Auth::Session& session
)
    : brandService(service),
      currentSession(session)
{
}

void StandardUI::handle(int brandId)
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

        renderer.displayStandard(data);

        // =========================================
        // ACTIONS
        // =========================================

        std::cout
            << "\n1. Add / Replace\n"
            << "2. Update Single Field\n"
            << "3. Clear Field\n"
            << "4. Back\n";

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

            renderer.displayStandard(data);

            BrandAggregate updateData;

            Standard s;

            std::cout
                << "\nEnter Position: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                s.setPosition(input);
            }

            std::cout
                << "Enter Brand Identity: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                s.setIdentity(input);
            }

            std::cout
                << "Enter CSR: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                s.setCSR(input);
            }

            std::cout
                << "Enter Quality: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                s.setQuality(input);
            }

            std::cout
                << "Enter Perception: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                s.setPerception(input);
            }

            updateData.standard = s;

            brandService.updateBrand(
                currentSession,
                brandId,
                updateData
            );

            UI::success(
                "Brand standard updated successfully."
            );

            pause();

            return;
        }

        // =========================================
        // UPDATE FIELD
        // =========================================

        else if(action == 2)
        {
            clearScreen();

            renderer.displayStandard(data);

            BrandAggregate updateData;

            Standard s;

            std::cout
                << "\nChoose Field:\n"
                << "1. Position\n"
                << "2. Brand Identity\n"
                << "3. CSR\n"
                << "4. Quality\n"
                << "5. Perception\n";

            int field =
                takeChoice();

            std::cout
                << "Enter New Value: ";

            std::getline(
                std::cin,
                input
            );

            switch(field)
            {
                case 1:
                    s.setPosition(input);
                    break;

                case 2:
                    s.setIdentity(input);
                    break;

                case 3:
                    s.setCSR(input);
                    break;

                case 4:
                    s.setQuality(input);
                    break;

                case 5:
                    s.setPerception(input);
                    break;

                default:
                {
                    UI::error(
                        "Invalid choice."
                    );

                    pause();

                    continue;
                }
            }

            updateData.standard = s;

            brandService.updateBrand(
                currentSession,
                brandId,
                updateData
            );

            UI::success(
                "Standard field updated successfully."
            );

            pause();

            return;
        }

        // =========================================
        // CLEAR FIELD
        // =========================================

        else if(action == 3)
        {
            clearScreen();

            renderer.displayStandard(data);

            BrandAggregate updateData;

            Standard s;

            std::cout
                << "\nChoose Field To Clear:\n"
                << "1. Position\n"
                << "2. Brand Identity\n"
                << "3. CSR\n"
                << "4. Quality\n"
                << "5. Perception\n";

            int field =
                takeChoice();

            switch(field)
            {
                case 1:
                {
                    s.setPosition(
                        std::nullopt
                    );

                    updateData.clearFields.insert(
                        "standard.position"
                    );

                    break;
                }

                case 2:
                {
                    s.setIdentity(
                        std::nullopt
                    );

                    updateData.clearFields.insert(
                        "standard.identity"
                    );

                    break;
                }

                case 3:
                {
                    s.setCSR(
                        std::nullopt
                    );

                    updateData.clearFields.insert(
                        "standard.csr"
                    );

                    break;
                }

                case 4:
                {
                    s.setQuality(
                        std::nullopt
                    );

                    updateData.clearFields.insert(
                        "standard.quality"
                    );

                    break;
                }

                case 5:
                {
                    s.setPerception(
                        std::nullopt
                    );

                    updateData.clearFields.insert(
                        "standard.perception"
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

            updateData.standard = s;

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