#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "Service/ReferencesService.h"

#include "CLI/BrandUI.h"
#include "CLI/BrandRenderUI.h"
#include "CLI/UI/TerminalLayout.h"

BrandUI::BrandUI(
    BrandService& service,
    ReferencesService& refService,
    const Auth::Session& session
)
    : brandService(service),
      referenceService(refService),
      currentSession(session),
      revenueUI(service, session),
      headsUI(service, refService, session),
      goalsUI(service, session),
      competitorsUI(service, session),
      genresUI(service, refService, session),
      audienceUI(service, refService, session),
      insightsUI(service, session),
      standardUI(service, session),
      newsFundingUI(service, session),
      brandCoreUI(service, session),
      pastInfluencerUI(service, session)
{
}

void BrandUI::run()
{
    showMenu();
}

void BrandUI::openBrand(
    int brandId
)
{
    viewBrandDetails(
        brandId
    );
}

void BrandUI::showMenu()
{
    int choice = 0;

    do
    {
        clearScreen();

        UI::title(
            "BRAND MODULE"
        );

        std::cout
            << "1. View Brands\n"
            << "2. Add Brand\n"
            << "3. Delete Brand\n"
            << "4. Back\n";

        std::cout
            << "\nEnter choice: ";

        choice = takeChoice();

        switch(choice)
        {
            case 1:
                viewBrands();
                break;

            case 2:
                createBrand();
                break;

            case 3:
                deleteBrand();
                break;

            case 4:
                return;

            default:
            {
                UI::error(
                    "Invalid choice."
                );

                pause();
            }
        }

    } while(true);
}

void BrandUI::viewBrands()
{
    while(true)
    {
        clearScreen();

        try
        {
            BrandPage page =
                brandService.getBrandsView(
                    currentSession,
                    "",
                    {},
                    "name",
                    "ASC",
                    1,
                    20
                );

            UI::title(
                "BRANDS LIST"
            );

            if(page.data.empty())
            {
                UI::warning(
                    "No brands found."
                );

                pause();

                return;
            }

            for(const auto& brand :
                page.data)
            {
                std::cout
                    << "["
                    << std::setw(3)
                    << brand.brand_id
                    << "] "
                    << brand.name
                    << "\n";
            }

            std::cout
                << "\nEnter Brand ID to open "
                << "(0 to back): ";

            int id =
                takeChoice();

            if(id == 0)
            {
                return;
            }

            viewBrandDetails(id);
        }

        catch(const std::exception& e)
        {
            UI::error(
                e.what()
            );

            pause();

            return;
        }
    }
}

void BrandUI::viewBrandDetails(
    int brandId
)
{
    while(true)
    {
        clearScreen();

        try
        {
            BrandAggregate data =
                brandService.getBrandById(
                    currentSession,
                    brandId
                );

            BrandRenderer renderer;

            renderer.displayBrandHeader(
                data
            );

            UI::title(
                "BRAND ACTIONS"
            );

            std::cout
                << "1. Edit Core\n"
                << "2. Goals\n"
                << "3. Heads\n"
                << "4. Competitors\n"
                << "5. Revenue\n"
                << "6. Genres\n"
                << "7. Audience\n"
                << "8. Insights\n"
                << "9. Standard\n"
                << "10. News/Funding\n"
                << "11. Past Influencer Campaigns\n"
                << "12. Back\n";

            std::cout
                << "\nEnter choice: ";

            int choice =
                takeChoice();

            if(choice == 12)
            {
                return;
            }

            clearScreen();

            handleSection(
                choice,
                brandId
            );
        }

        catch(const std::exception& e)
        {
            UI::error(
                e.what()
            );

            pause();

            return;
        }
    }
}

void BrandUI::handleSection(
    int choice,
    int brandId
)
{
    switch(choice)
    {
        case 1:
            brandCoreUI.handle(brandId);
            break;

        case 2:
            goalsUI.handle(brandId);
            break;

        case 3:
            headsUI.handle(brandId);
            break;

        case 4:
            competitorsUI.handle(brandId);
            break;

        case 5:
            revenueUI.handle(brandId);
            break;

        case 6:
            genresUI.handle(brandId);
            break;

        case 7:
            audienceUI.handle(brandId);
            break;

        case 8:
            insightsUI.handle(brandId);
            break;

        case 9:
            standardUI.handle(brandId);
            break;

        case 10:
            newsFundingUI.handle(brandId);
            break;

        case 11:
            pastInfluencerUI.handle(brandId);
            break;

        default:
        {
            UI::error(
                "Invalid section choice."
            );

            pause();

            break;
        }
    }
}

void BrandUI::deleteBrand()
{
    clearScreen();

    UI::title(
        "DELETE BRAND"
    );

    std::cout
        << "Enter Brand ID to delete "
        << "(0 to back): ";

    int id =
        takeChoice();

    if(id == 0)
    {
        return;
    }

    try
    {
        brandService.deleteBrand(
            currentSession,
            id
        );

        UI::success(
            "Brand deleted successfully."
        );
    }

    catch(const std::exception& e)
    {
        UI::error(
            e.what()
        );
    }

    pause();
}

void BrandUI::createBrand()
{
    clearScreen();

    try
    {
        BrandAggregate data;

        Brand brand;

        std::string input;

        UI::title(
            "CREATE BRAND"
        );

        std::cout
            << "Enter Brand Name: ";

        std::getline(
            std::cin,
            input
        );

        if(input.empty())
        {
            UI::error(
                "Brand name cannot be empty."
            );

            pause();

            return;
        }

        brand.setName(
            input
        );

        std::cout
            << "Enter Company Type: ";

        std::getline(
            std::cin,
            input
        );

        if(!input.empty())
        {
            brand.setCompanyType(
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
            brand.setPsychographics(
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
            brand.setGenreDescription(
                input
            );
        }

        data.brand = brand;

        int brandId =
            brandService.createBrand(
                currentSession,
                data
            );

        UI::success(
            "Brand created successfully."
        );

        std::cout
            << "\nOpening Brand Details...\n";

        pause();

        viewBrandDetails(
            brandId
        );

        return;
    }

    catch(const std::exception& e)
    {
        UI::error(
            e.what()
        );
    }

    pause();
}