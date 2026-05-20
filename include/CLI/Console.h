// ================================
// include/CLI/Console.h
// UPDATED VERSION
// ================================

#pragma once

#include <optional>

#include "Db/Analytics/AnalyticsRepository.h"
#include "Db/DbManager.h"

#include "Auth/AuthService.h"

#include "Service/BrandService.h"
#include "Service/CreatorService.h"
#include "Service/ReferencesService.h"
#include "Service/DealService.h"
#include "Service/AnalyticsService.h"

#include "CLI/BrandUI.h"
#include "CLI/ReferenceManagementUI.h"
#include "CLI/Query/SearchFilterUI.h"

#include "CLI/ProfileUI.h"
#include "CLI/DealUI.h"
#include "CLI/UI/TerminalLayout.h"
#include "CLI/BaseUI.h"
class Console : public BaseUI
{
public:

    Console(
        DbManager& db,
        BrandService& brandService,
        CreatorService& creatorService,
        DealService& dealService,
        ReferencesService& refService,
        AnalyticsService& analyticsService
    );

    void run();

private:

    DbManager& db;

    BrandService& brandService;

    CreatorService& creatorService;

    ReferencesService& referenceService;

    AnalyticsService& analyticsService;

    DealService& dealService; 

    std::optional<Auth::Session> currentSession;

    void welcomeMenu();

    void loginMenu();

    void dashboardMenu();

    void brandsMenu();

    void creatorsMenu();

    void dealsMenu();

    void analyticsMenu();

    void searchMenu();

    void profileMenu();

    void referenceManagementMenu();

    int takeChoice();

    void pause();
};