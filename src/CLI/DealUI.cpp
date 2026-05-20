// DealUI.cpp

#include "CLI/DealUI.h"

#include <iostream>
#include <iomanip>
#include <limits>

#include "Models/Helpers/Date.h"

DealUI::DealUI(
    DealService& dealService,
    BrandService& brandService,
    CreatorService& creatorService,
    const Auth::Session& session
)
    : dealService(dealService),
      brandService(brandService),
      creatorService(creatorService),
      currentSession(session)
{
}

void DealUI::run()
{
    int choice;

    do {

        clearScreen();

        showDeals();

        UI::title("DEALS");

        std::cout
            << "1. Create New Deal\n"
            << "2. Open Existing Deal\n"
            << "3. Delete Deal\n"
            << "0. Back\n";

        std::cout << "Enter choice: ";

        choice = takeChoice();

        switch(choice)
        {
            case 1:
                createDeal();
                break;

            case 2:
                openDeal();
                break;

            case 3:
                deleteDeal();
                break;

            case 0:
                return;

            default:
                UI::error("Invalid choice.");
                pause();
        }

    } while(true);
}

void DealUI::showDeals()
{
    DealPage page;

    try
    {
        page =
            dealService.getDealsView(
                currentSession
            );
    }
    catch(const std::exception& e)
    {
        UI::error(e.what());
        pause();
        return;
    }

    std::cout << "\n===============================================================\n";
    std::cout << "                           DEALS                              \n";
    std::cout << "===============================================================\n\n";

    std::cout
        << std::left
        << std::setw(10) << "ID"
        << std::setw(25) << "Brand"
        << std::setw(25) << "Creator"
        << std::setw(15) << "Date"
        << "\n";

    std::cout
        << "---------------------------------------------------------------\n";

    for(const auto& deal : page.data)
    {
        std::cout
            << std::left
            << std::setw(10) << deal.deal_id
            << std::setw(25) << deal.brand_name
            << std::setw(25) << deal.creator_name
            << std::setw(15) << deal.date
            << "\n";
    }
}

void DealUI::createDeal()
{
    clearScreen();

    Deal deal;

    std::string input;

    UI::title("CREATE DEAL");

    BrandPage brandPage =
        brandService.getBrandsView(
            currentSession,
            "",
            {},
            "name",
            "ASC",
            1,
            20
        );

    UI::title("BRANDS");

    for(const auto& brand : brandPage.data)
    {
        std::cout
            << "["
            << brand.brand_id
            << "] "
            << brand.name
            << "\n";
    }

    std::cout << "\nEnter Brand ID: ";

    deal.brand_id = takeChoice();

    std::vector<Creator> creators =
        creatorService.getAllCreators(
            currentSession
        );

    UI::title("CREATORS");

    for(const auto& creator : creators)
    {
        std::cout
            << "["
            << creator.getId()
            << "] "
            << creator.getName()
            << "\n";
    }

    std::cout << "\nEnter Creator ID: ";

    deal.creator_id = takeChoice();

    std::cout << "\nIs this a lead? (yes/no): ";

    std::getline(std::cin, input);

    deal.is_lead = parseYesNo(input);

    deal.is_connected = false;
    deal.is_deal_done = false;
    deal.post_uploaded = false;
    deal.payment_cleared = false;
    deal.is_deleted = false;

    std::cout << "Enter date (YYYY-MM-DD): ";

    std::getline(std::cin, input);

    try
    {
        Date::fromString(input);
        deal.date = input;
    }
    catch(const std::exception& e)
    {
        UI::error(e.what());
        pause();
        return;
    }

    try
    {
        int dealId =
            dealService.createDeal(
                currentSession,
                deal
            );

        UI::success(
            "Deal created successfully with ID: "
            + std::to_string(dealId)
        );
    }

    catch(const std::exception& e)
    {
        UI::error(e.what());
    }

    pause();
}

void DealUI::openDeal()
{
    std::cout << "\nEnter Deal ID (0 to back): ";

    int dealId = takeChoice();

    if(dealId == 0)
    {
        return;
    }

    dealMenu(dealId);
}

void DealUI::deleteDeal()
{
    std::cout << "\nEnter Deal ID to delete (0 to back): ";

    int dealId = takeChoice();

    if(dealId == 0)
    {
        return;
    }

    try
    {
        dealService.deleteDeal(
            currentSession,
            dealId
        );

        UI::success("Deal deleted successfully.");
    }

    catch(const std::exception& e)
    {
        UI::error(e.what());
    }

    pause();
}

void DealUI::dealMenu(int dealId)
{
    while(true)
    {
        clearScreen();

        DealPage page =
            dealService.getDealsView(
                currentSession
            );

        bool found = false;

        DealView selectedDeal;

        for(const auto& deal : page.data)
        {
            if(deal.deal_id == dealId)
            {
                selectedDeal = deal;
                found = true;
                break;
            }
        }

        if(!found)
        {
            UI::error("Deal not found.");
            pause();
            return;
        }

        renderDealDetails(selectedDeal);

        std::cout << "\n1. Update Status\n";
        std::cout << "0. Back\n";

        std::cout << "Enter choice: ";

        int choice = takeChoice();

        switch(choice)
        {
            case 1:
                updateDealField(dealId);
                break;

            case 0:
                return;

            default:
                UI::error("Invalid choice.");
                pause();
        }
    }
}

void DealUI::updateDealField(int dealId)
{
    clearScreen();

    std::cout << "\n===== UPDATE DEAL =====\n";

    std::cout << "1. Lead\n";
    std::cout << "2. Connected\n";
    std::cout << "3. Deal Done\n";
    std::cout << "4. Post Uploaded\n";
    std::cout << "5. Payment Cleared\n";
    std::cout << "6. Date\n";
    std::cout << "0. Back\n";

    std::cout << "Enter choice: ";

    int choice = takeChoice();

    if(choice == 0)
    {
        return;
    }

    DealUpdate updateData;

    std::string input;

    try
    {
        switch(choice)
        {
            case 1:
                std::cout << "Lead (yes/no): ";
                std::getline(std::cin, input);
                updateData.is_lead = parseYesNo(input);
                break;

            case 2:
                std::cout << "Connected (yes/no): ";
                std::getline(std::cin, input);
                updateData.is_connected = parseYesNo(input);
                break;

            case 3:
                std::cout << "Deal Done (yes/no): ";
                std::getline(std::cin, input);
                updateData.is_deal_done = parseYesNo(input);
                break;

            case 4:
                std::cout << "Post Uploaded (yes/no): ";
                std::getline(std::cin, input);
                updateData.post_uploaded = parseYesNo(input);
                break;

            case 5:
                std::cout << "Payment Cleared (yes/no): ";
                std::getline(std::cin, input);
                updateData.payment_cleared = parseYesNo(input);
                break;

            case 6:
                std::cout << "Enter date (YYYY-MM-DD): ";
                std::getline(std::cin, input);
                try
                {
                    Date::fromString(input);
                    updateData.date = input;
                }
                catch(const std::exception& e)
                {
                    UI::error(e.what());
                    pause();
                    return;
                }
                break;

            default:
                UI::error("Invalid choice.");
                pause();
                return;
        }

        dealService.updateDeal(
            currentSession,
            dealId,
            updateData
        );

        std::cout << "\nDeal updated successfully.\n";
    }

    catch(const std::exception& e)
    {
        UI::error(e.what());
    }

    pause();
}

void DealUI::renderDealDetails(const DealView& deal)
{
    std::cout << "\n========== DEAL #"
              << deal.deal_id
              << " =========="
              << "\n\n";

    std::cout
        << "Brand   : "
        << deal.brand_name
        << "\n";

    std::cout
        << "Creator : "
        << deal.creator_name
        << "\n\n";

    std::cout
        << statusSymbol(deal.is_lead)
        << " Lead\n";

    std::cout
        << statusSymbol(deal.is_connected)
        << " Connected\n";

    std::cout
        << statusSymbol(deal.is_deal_done)
        << " Deal Done\n";

    std::cout
        << statusSymbol(deal.post_uploaded)
        << " Post Uploaded\n";

    std::cout
        << statusSymbol(deal.payment_cleared)
        << " Payment Cleared\n";

    std::cout
        << "\nDate : "
        << deal.date
        << "\n";
}

std::string DealUI::statusSymbol(bool value)
{
    return value ? "[✓]" : "[ ]";
}

bool DealUI::parseYesNo(const std::string& input)
{
    return (
        input == "yes" ||
        input == "Yes" ||
        input == "y" ||
        input == "Y"
    );
}

int DealUI::takeChoice()
{
    int choice;

    while(!(std::cin >> choice))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout << "Enter valid number: ";
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    return choice;
}

void DealUI::pause()
{
    std::cout << "Press Enter to continue...";

    std::cin.get();
}

void DealUI::clearScreen()
{
    system("clear");
}