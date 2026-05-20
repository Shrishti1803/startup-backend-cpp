#include "CLI/Query/QueryResultsUI.h"
#include "CLI/UI/TerminalLayout.h"

#include <iostream>
#include <iomanip>

QueryResultsUI::QueryResultsUI(
    const BrandPage& brandPage,
    const CreatorPage& creatorPage,
    BrandUI& brandUI,
    CreatorUI& creatorUI
)
    : brandPage(brandPage),
      creatorPage(creatorPage),
      brandUI(brandUI),
      creatorUI(creatorUI)
{
}

void QueryResultsUI::run()
{
    int choice;

    do
    {
        clearScreen();

        renderResults();

        UI::title("SEARCH ACTIONS");

        std::cout
            << "1. Open Brand\n"
            << "2. Open Creator\n"
            << "3. Back\n";

        std::cout
            << "\nEnter choice: ";

        choice = takeChoice();

        switch(choice)
        {
            case 1:
                openBrand();
                break;

            case 2:
                openCreator();
                break;

            case 3:
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

void QueryResultsUI::renderResults()
{
    UI::title("SEARCH RESULTS");

    // ================= BRANDS =================

    UI::title("BRANDS");

    if(brandPage.data.empty())
    {
        UI::warning(
            "No brands found."
        );
    }

    else
    {
        for(const auto& brand :
            brandPage.data)
        {
            std::cout
                << "["
                << std::setw(3)
                << brand.brand_id
                << "] "
                << brand.name
                << "\n";
        }
    }

    // ================= CREATORS =================

    UI::title("CREATORS");

    if(creatorPage.data.empty())
    {
        UI::warning(
            "No creators found."
        );
    }

    else
    {
        for(const auto& creator :
            creatorPage.data)
        {
            std::cout
                << "["
                << std::setw(3)
                << creator.creator_id
                << "] "
                << creator.name
                << "\n";
        }
    }

    // ================= SUMMARY =================

    std::cout
        << "\n----------------------------------\n";

    std::cout
        << " Brands Found   : "
        << brandPage.data.size()
        << "\n";

    std::cout
        << " Creators Found : "
        << creatorPage.data.size()
        << "\n";

    std::cout
        << "----------------------------------\n";
}

void QueryResultsUI::openBrand()
{
    if(brandPage.data.empty())
    {
        UI::warning(
            "No brands available to open."
        );

        pause();

        return;
    }

    std::cout
        << "\nEnter Brand ID "
        << "(0 to cancel): ";

    int brandId =
        takeChoice();

    if(brandId == 0)
    {
        return;
    }

    brandUI.openBrand(
        brandId
    );
}

void QueryResultsUI::openCreator()
{
    if(creatorPage.data.empty())
    {
        UI::warning(
            "No creators available to open."
        );

        pause();

        return;
    }

    std::cout
        << "\nEnter Creator ID "
        << "(0 to cancel): ";

    int creatorId =
        takeChoice();

    if(creatorId == 0)
    {
        return;
    }

    creatorUI.openCreatorById(
        creatorId
    );
}