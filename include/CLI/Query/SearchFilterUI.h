#pragma once

#include "CLI/BaseUI.h"

#include "Service/BrandService.h"
#include "Service/CreatorService.h"
#include "Service/ReferencesService.h"

#include "Auth/AuthService.h"
#include "CLI/UI/TerminalLayout.h"
#include "CLI/BrandUI.h"
#include "CLI/Creator/CreatorUI.h"

class SearchFilterUI : public BaseUI
{
private:

    BrandService& brandService;

    CreatorService& creatorService;

    ReferencesService& referenceService;

    BrandUI& brandUI;

    CreatorUI& creatorUI;

    const Auth::Session currentSession;

public:

    SearchFilterUI(
        BrandService& brandService,
        CreatorService& creatorService,
        ReferencesService& referenceService,
        BrandUI& brandUI,
        CreatorUI& creatorUI,
        const Auth::Session& session
    );

    void run();

private:

    void showMenu();

    void search();

    void applyFilters();
};