// ================================
// include/CLI/BrandUI.h
// ================================
#pragma once

#include "Service/BrandService.h"
#include "Service/ReferencesService.h"
#include "Auth/AuthService.h"
#include "CLI/RevenueUI.h"
#include "CLI/BaseUI.h"
#include "CLI/HeadsUI.h"
#include "CLI/GoalsUI.h"
#include "CLI/CompetitorsUI.h"
#include "CLI/GenresUI.h"
#include "CLI/AudienceUI.h"
#include "CLI/InsightsUI.h"
#include "CLI/StandardUI.h"
#include "CLI/NewsFundingUI.h"
#include "CLI/BrandCoreUI.h"
#include "CLI/PastInfluencerCampUI.h"
#include "CLI/UI/TerminalLayout.h"
class BrandUI : public BaseUI
{
public:
    BrandUI(BrandService& service, ReferencesService& refService, const Auth::Session& session);
    void run();
    void openBrand(int brandId);

private:
    BrandService& brandService;
    ReferencesService& referenceService;
    const Auth::Session currentSession;
    RevenueUI revenueUI;
    HeadsUI headsUI;
    GoalsUI goalsUI;
    CompetitorsUI competitorsUI;
    GenresUI genresUI;
    AudienceUI audienceUI;
    InsightsUI insightsUI;
    StandardUI standardUI;
    NewsFundingUI newsFundingUI;
    BrandCoreUI brandCoreUI;
    PastInfluencerCampUI pastInfluencerUI;

    void showMenu();
    void viewBrands();
    void viewBrandDetails(int brandId);
    void deleteBrand();
    void createBrand();
    void deleteEntity(int brandId, int section);
    void handleSection(int choice, int brandId);
};