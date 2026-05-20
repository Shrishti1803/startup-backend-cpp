#include "CLI/AudienceUI.h"
#include "CLI/UI/TerminalLayout.h"

#include <limits>
#include <iostream>

#include "Models/References/TargetAudience.h"
#include "Models/Brand/Brand.h"

AudienceUI::AudienceUI(
    BrandService& service,
    ReferencesService& refService,
    const Auth::Session& session
)
    : brandService(service),
      referencesService(refService),
      currentSession(session)
{
}

void AudienceUI::handle(int brandId)
{
    int action;

    do
    {
        clearScreen();

        UI::title("TARGET AUDIENCE");

        // =========================================
        // CURRENT BRAND TARGET AUDIENCE
        // =========================================

        auto brand =
            brandService.getBrandById(
                currentSession,
                brandId
            );

        UI::title(
            "CURRENT BRAND TARGET AUDIENCE"
        );

        if(brand.targetAudience.empty())
        {
            UI::warning(
                "No linked target audience."
            );
        }

        else
        {
            for(const auto& audience :
                brand.targetAudience)
            {
                std::cout
                    << "["
                    << audience.getId()
                    << "] "
                    << audience.getName()
                    << "\n";
            }
        }

        // =========================================
        // ALL AVAILABLE TARGET AUDIENCE
        // =========================================

        auto allAudience =
            referencesService.getAllTargetAudience(
                currentSession
            );

        UI::title(
            "ALL AVAILABLE TARGET AUDIENCE"
        );

        if(allAudience.empty())
        {
            UI::warning(
                "No target audience available."
            );
        }

        else
        {
            for(const auto& audience :
                allAudience)
            {
                std::cout
                    << "["
                    << audience.getId()
                    << "] "
                    << audience.getName()
                    << "\n";
            }
        }

        // =========================================
        // ACTIONS
        // =========================================

        UI::title(
            "AUDIENCE MANAGEMENT"
        );

        std::cout
            << "1. Link Existing\n"
            << "2. Create New Audience\n"
            << "3. Update Links (Replace Full Set)\n"
            << "4. Remove Link\n"
            << "5. Back\n";

        action = takeChoice();

        // =========================================
        // LINK EXISTING
        // =========================================

        if(action == 1)
        {
            int id;

            std::cout
                << "\nEnter Audience ID to link "
                << "(0 to back): ";

            id = takeChoice();

            if(id == 0)
            {
                return;
            }

            auto brand =
                brandService.getBrandById(
                    currentSession,
                    brandId
                );

            BrandAggregate data;

            // =========================================
            // Preserve existing audience
            // =========================================

            data.targetAudience =
                brand.targetAudience;

            // =========================================
            // Add new audience
            // =========================================

            TargetAudience a;

            a.setId(id);

            data.targetAudience
                .push_back(a);

            brandService.updateBrand(
                currentSession,
                brandId,
                data
            );

            UI::success(
                "Audience linked successfully."
            );

            pause();
        }

        // =========================================
        // CREATE NEW AUDIENCE
        // =========================================

        else if(action == 2)
        {
            std::string name;

            std::cout
                << "\nEnter New Audience Name: ";

            std::getline(
                std::cin,
                name
            );

            if(name.empty())
            {
                UI::error(
                    "Audience name cannot be empty."
                );

                pause();

                continue;
            }

            referencesService.createTargetAudience(
                currentSession,
                name
            );

            UI::success(
                "Audience created successfully."
            );

            pause();
        }

        // =========================================
        // UPDATE LINKS (FULL REPLACE)
        // =========================================

        else if(action == 3)
        {
            UI::warning(
                "This will replace all existing audience links."
            );

            std::cout
                << "\nEnter Audience IDs "
                << "(space separated, use -1 to finish):\n";

            std::vector<TargetAudience> newSet;

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
                        "Please enter valid numeric IDs."
                    );

                    pause();

                    return;
                }

                if(id == -1)
                {
                    break;
                }

                TargetAudience a;

                a.setId(id);

                newSet.push_back(a);
            }

            std::cin.ignore(
                std::numeric_limits<
                    std::streamsize
                >::max(),
                '\n'
            );

            BrandAggregate data;

            data.targetAudience =
                newSet;

            brandService.updateBrand(
                currentSession,
                brandId,
                data
            );

            UI::success(
                "Audience links updated successfully."
            );

            pause();
        }

        // =========================================
        // REMOVE LINK
        // =========================================

        else if(action == 4)
        {
            int id;

            std::cout
                << "\nEnter Audience ID to remove "
                << "(0 to back): ";

            id = takeChoice();

            if(id == 0)
            {
                return;
            }

            BrandAggregate data;

            TargetAudience a;

            a.setId(id);

            data.targetAudience
                .push_back(a);

            brandService.deletePartial(
                currentSession,
                brandId,
                data
            );

            UI::success(
                "Audience link removed successfully."
            );

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