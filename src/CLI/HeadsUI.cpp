#include "CLI/HeadsUI.h"

#include <iostream>
#include <limits>

#include "Db/Brand/BrandAggregateRepository.h"

#include "Models/Brand/BrandHead.h"
#include "Models/Brand/HeadContact.h"
#include "Models/Brand/Position.h"

HeadsUI::HeadsUI(
    BrandService& service,
    ReferencesService& refService,
    const Auth::Session& session
)
    : brandService(service),
      currentSession(session),
      referencesService(refService)
{
}

void HeadsUI::handle(
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

        renderer.displayCompactHeader(
            data
        );

        renderer.displayHeads(
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

            renderer.displayHeads(
                data
            );

            BrandAggregate updateData;

            BrandHead h;

            // ===== NAME =====

            std::cout
                << "\nEnter Name: ";

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

            h.setName(
                input
            );

            // ===== POSITION =====

            auto positions =
                referencesService.getAllPositions(
                    currentSession
                );

            std::cout
                << "\n===== AVAILABLE POSITIONS =====\n";

            for(const auto& p : positions)
            {
                std::cout
                    << "["
                    << p.getId()
                    << "] "
                    << p.getName()
                    << "\n";
            }

            std::cout
                << "\nEnter Position ID "
                << "(0 to create new): ";

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
                    "Invalid position id."
                );

                pause();

                continue;
            }

            int positionId =
                std::stoi(input);

            Position p;

            if(positionId == 0)
            {
                std::string newPosition;

                std::cout
                    << "Enter New Position Name: ";

                std::getline(
                    std::cin,
                    newPosition
                );

                if(newPosition.empty())
                {
                    UI::error(
                        "Position name cannot be empty."
                    );

                    pause();

                    continue;
                }

                int newId =
                    referencesService.createPosition(
                        currentSession,
                        newPosition
                    );

                p.setId(newId);
                p.setName(newPosition);
            }

            else
            {
                p.setId(positionId);
            }

            h.setPosition(p);

            // ===== LINKEDIN =====

            std::cout
                << "Enter LinkedIn (optional): ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                h.setLinkedIn(
                    input
                );
            }

            // ===== PERSONAL MAIL =====

            std::cout
                << "Enter Personal Mail: ";

            std::getline(
                std::cin,
                input
            );

            if(input.empty())
            {
                UI::error(
                    "Personal mail cannot be empty."
                );

                pause();

                continue;
            }

            h.setPersonalMail(
                input
            );

            // ===== OFFICIAL MAIL =====

            std::cout
                << "Enter Official Mail: ";

            std::getline(
                std::cin,
                input
            );

            if(input.empty())
            {
                UI::error(
                    "Official mail cannot be empty."
                );

                pause();

                continue;
            }

            h.setOfficialMail(
                input
            );

            // ===== CONTACTS =====

            std::cout
                << "Add Contact? (y/n): ";

            std::getline(
                std::cin,
                input
            );

            while(
                input == "y" ||
                input == "Y"
            )
            {
                HeadContact c;

                std::string val;
                std::string type;

                std::cout
                    << "Contact Value: ";

                std::getline(
                    std::cin,
                    val
                );

                if(val.empty())
                {
                    UI::error(
                        "Contact value cannot be empty."
                    );

                    pause();

                    continue;
                }

                c.setValue(val);

                std::cout
                    << "Contact Type: ";

                std::getline(
                    std::cin,
                    type
                );

                if(!type.empty())
                {
                    c.setType(type);
                }

                h.addContact(c);

                std::cout
                    << "Add another? (y/n): ";

                std::getline(
                    std::cin,
                    input
                );
            }

            updateData.heads
                .push_back(h);

            try
            {
                brandService.updateBrand(
                    currentSession,
                    brandId,
                    updateData
                );

                UI::success(
                    "Head added successfully."
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

            renderer.displayHeads(
                data
            );

            if(data.heads.empty())
            {
                UI::warning(
                    "No heads available to update."
                );

                pause();

                continue;
            }

            BrandAggregate updateData;

            int id;

            std::cout
                << "\nEnter Head ID "
                << "(0 to cancel): ";

            id = takeChoice();

            if(id == 0)
            {
                continue;
            }

            BrandHead h;

            h.setId(id);

            std::cout
                << "\n--- Update Head Field ---\n";

            std::cout
                << "1. Name\n"
                << "2. Position\n"
                << "3. LinkedIn\n"
                << "4. Personal Mail\n"
                << "5. Official Mail\n";

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

                    h.setName(input);

                    break;
                }

                // ===== POSITION =====

                case 2:
                {
                    auto positions =
                        referencesService.getAllPositions(
                            currentSession
                        );

                    std::cout
                        << "\n===== AVAILABLE POSITIONS =====\n";

                    for(const auto& p : positions)
                    {
                        std::cout
                            << "["
                            << p.getId()
                            << "] "
                            << p.getName()
                            << "\n";
                    }

                    std::cout
                        << "\nEnter New Position ID "
                        << "(0 to create new): ";

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
                            "Invalid position id."
                        );

                        pause();

                        continue;
                    }

                    int positionId =
                        std::stoi(input);

                    Position p;

                    if(positionId == 0)
                    {
                        std::string newPosition;

                        std::cout
                            << "Enter New Position Name: ";

                        std::getline(
                            std::cin,
                            newPosition
                        );

                        if(newPosition.empty())
                        {
                            UI::error(
                                "Position name cannot be empty."
                            );

                            pause();

                            continue;
                        }

                        int newId =
                            referencesService.createPosition(
                                currentSession,
                                newPosition
                            );

                        p.setId(newId);
                        p.setName(newPosition);
                    }

                    else
                    {
                        p.setId(positionId);
                    }

                    h.setPosition(p);

                    break;
                }

                // ===== LINKEDIN =====

                case 3:
                {
                    std::cout
                        << "Enter New LinkedIn URL: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "LinkedIn cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    h.setLinkedIn(input);

                    break;
                }

                // ===== PERSONAL MAIL =====

                case 4:
                {
                    std::cout
                        << "Enter New Personal Mail: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Personal mail cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    h.setPersonalMail(input);

                    break;
                }

                // ===== OFFICIAL MAIL =====

                case 5:
                {
                    std::cout
                        << "Enter New Official Mail: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Official mail cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    h.setOfficialMail(input);

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

            updateData.heads
                .push_back(h);

            try
            {
                brandService.updateBrand(
                    currentSession,
                    brandId,
                    updateData
                );

                UI::success(
                    "Head updated successfully."
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

            renderer.displayHeads(
                data
            );

            if(data.heads.empty())
            {
                UI::warning(
                    "No heads available to delete."
                );

                pause();

                continue;
            }

            int id;

            std::cout
                << "\nEnter Head ID to delete "
                << "(0 to cancel): ";

            id = takeChoice();

            if(id == 0)
            {
                continue;
            }

            BrandAggregate deleteData;

            BrandHead h;

            h.setId(id);

            deleteData.heads
                .push_back(h);

            try
            {
                brandService.deletePartial(
                    currentSession,
                    brandId,
                    deleteData
                );

                UI::success(
                    "Head deleted successfully."
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