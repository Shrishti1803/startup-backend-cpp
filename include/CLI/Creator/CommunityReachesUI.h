#pragma once

#include "CLI/BaseUI.h"

#include "Service/CreatorService.h"

#include "Auth/AuthService.h"
#include "CLI/Creator/CreatorRenderUI.h"
#include "Service/ReferencesService.h"
#include "CLI/UI/TerminalLayout.h"
class CommunityReachesUI : public BaseUI
{
private:

    CreatorService& creatorService;
    ReferencesService& referencesService;
    Auth::Session currentSession;

public:

    CommunityReachesUI(
        CreatorService& service,
        ReferencesService& referenceService,
        const Auth::Session& session
    );

    void handle(int creatorId);
};