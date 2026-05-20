#pragma once

#include "CLI/BaseUI.h"

#include "Service/CreatorService.h"
#include "Service/ReferencesService.h"

#include "Auth/AuthService.h"
#include "CLI/UI/TerminalLayout.h"
#include "CLI/Creator/CreatorRenderUI.h"
#include "CLI/Creator/CreatorCoreUI.h"
#include "CLI/Creator/CommunityReachesUI.h"
#include "CLI/Creator/ProfileAccountUI.h"

class CreatorUI : public BaseUI
{
private:

    CreatorService& creatorService;

    ReferencesService& referenceService;

    Auth::Session currentSession;

    CreatorRenderUI creatorRenderer;

    ProfileAccountUI profileAccountUI;

public:

    CreatorUI(
        CreatorService& creatorService,
        ReferencesService& referenceService,
        const Auth::Session& session
    );
    void openCreatorById(int creatorId);
    void run();

private:

    void creatorMenu(int creatorId);

    void createCreator();

    void openCreator();

    void deleteCreator();

};