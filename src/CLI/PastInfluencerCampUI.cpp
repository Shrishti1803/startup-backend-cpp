#include "CLI/PastInfluencerCampUI.h"

#include <iostream>

#include "Models/Brand/PastInfluencerCamp.h"
#include "Db/Brand/BrandAggregateRepository.h"
#include "CLI/BrandRenderUI.h"

PastInfluencerCampUI::PastInfluencerCampUI(
    BrandService& service,
    const Auth::Session& session
)
    : brandService(service),
      currentSession(session)
{
}

void PastInfluencerCampUI::handle(
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

        renderer.displayPastInfluencerCamps(
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

            renderer.displayPastInfluencerCamps(
                data
            );

            BrandAggregate updateData;

            PastInfluencerCamp p;

            std::cout
                << "\nEnter Ambassadors: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                p.setAmbassadors(
                    input
                );
            }

            std::cout
                << "Enter Campaigns: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                p.setCampaigns(
                    input
                );
            }

            std::cout
                << "Enter Strategy: ";

            std::getline(
                std::cin,
                input
            );

            if(!input.empty())
            {
                p.setStrategy(
                    input
                );
            }

            if(
                !p.getAmbassadors().has_value() &&
                !p.getCampaigns().has_value() &&
                !p.getStrategy().has_value()
            )
            {
                UI::error(
                    "At least one field is required."
                );

                pause();

                continue;
            }

            updateData.pastInfluencerCamps
                .push_back(p);

            try
            {
                brandService.updateBrand(
                    currentSession,
                    brandId,
                    updateData
                );

                UI::success(
                    "Past influencer campaign added successfully."
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

            renderer.displayPastInfluencerCamps(
                data
            );

            if(data.pastInfluencerCamps.empty())
            {
                UI::error(
                    "No campaign records available."
                );

                pause();

                continue;
            }

            BrandAggregate updateData;

            int id;

            std::cout
                << "\nEnter Campaign ID: ";

            id = takeChoice();

            PastInfluencerCamp p;

            p.setId(id);

            clearScreen();

            renderer.displayPastInfluencerCamps(
                data
            );

            std::cout
                << "\n--- Update Campaign Field ---\n";

            std::cout
                << "1. Ambassadors\n"
                << "2. Campaigns\n"
                << "3. Strategy\n";

            std::cout
                << "\nEnter choice: ";

            int fieldChoice =
                takeChoice();

            switch(fieldChoice)
            {
                // ===== AMBASSADORS =====

                case 1:
                {
                    std::cout
                        << "Enter New Ambassadors: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Ambassadors cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    p.setAmbassadors(
                        input
                    );

                    break;
                }

                // ===== CAMPAIGNS =====

                case 2:
                {
                    std::cout
                        << "Enter New Campaigns: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Campaigns cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    p.setCampaigns(
                        input
                    );

                    break;
                }

                // ===== STRATEGY =====

                case 3:
                {
                    std::cout
                        << "Enter New Strategy: ";

                    std::getline(
                        std::cin,
                        input
                    );

                    if(input.empty())
                    {
                        UI::error(
                            "Strategy cannot be empty."
                        );

                        pause();

                        continue;
                    }

                    p.setStrategy(
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

            updateData.pastInfluencerCamps
                .push_back(p);

            try
            {
                brandService.updateBrand(
                    currentSession,
                    brandId,
                    updateData
                );

                UI::success(
                    "Past influencer campaign updated successfully."
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

            renderer.displayPastInfluencerCamps(
                data
            );

            if(data.pastInfluencerCamps.empty())
            {
                UI::error(
                    "No campaign records available."
                );

                pause();

                continue;
            }

            BrandAggregate deleteData;

            int id;

            std::cout
                << "\nEnter Campaign ID to delete: ";

            id = takeChoice();

            PastInfluencerCamp p;

            p.setId(id);

            deleteData.pastInfluencerCamps
                .push_back(p);

            try
            {
                brandService.deletePartial(
                    currentSession,
                    brandId,
                    deleteData
                );

                UI::success(
                    "Past influencer campaign deleted successfully."
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