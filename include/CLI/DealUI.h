// DealUI.h
#pragma once

#include "Service/DealService.h"
#include "Service/BrandService.h"
#include "Service/CreatorService.h"
#include "Auth/AuthService.h"
#include "CLI/UI/TerminalLayout.h"
#include "CLI/UI/TerminalLayout.h"
class DealUI {

private:
    DealService& dealService;
    BrandService& brandService;
    CreatorService& creatorService;

    const Auth::Session& currentSession;

public:
    DealUI(
        DealService& dealService,
        BrandService& brandService,
        CreatorService& creatorService,
        const Auth::Session& session
    );

    void run();

private:
    void showDeals();

    void createDeal();

    void openDeal();

    void deleteDeal();

    void dealMenu(int dealId);

    void updateDealField(int dealId);

    void renderDealDetails(const DealView& deal);

    std::string statusSymbol(bool value);

    bool parseYesNo(const std::string& input);

    int takeChoice();

    void pause();

    void clearScreen();
};