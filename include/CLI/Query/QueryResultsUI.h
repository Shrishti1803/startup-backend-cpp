#pragma once

#include "CLI/BaseUI.h"

#include "CLI/BrandUI.h"
#include "CLI/Creator/CreatorUI.h"
#include "CLI/UI/TerminalLayout.h"
#include "Service/BrandService.h"
#include "Service/CreatorService.h"

class QueryResultsUI : public BaseUI
{
private:

    BrandPage brandPage;

    CreatorPage creatorPage;

    BrandUI& brandUI;

    CreatorUI& creatorUI;

public:

    QueryResultsUI(
        const BrandPage& brandPage,
        const CreatorPage& creatorPage,
        BrandUI& brandUI,
        CreatorUI& creatorUI
    );

    void run();

private:

    void renderResults();

    void openBrand();

    void openCreator();
};