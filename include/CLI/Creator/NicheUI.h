#pragma once

#include "CLI/BaseUI.h"

#include "Service/CreatorService.h"
#include "Service/ReferencesService.h"
#include "CLI/UI/TerminalLayout.h"
#include "Auth/AuthService.h"
#include "CLI/Creator/CreatorRenderUI.h"
class NicheUI : public BaseUI
{
private:

    CreatorService& creatorService;

    ReferencesService& referencesService;

    Auth::Session currentSession;

public:

    NicheUI(
        CreatorService& creatorService,
        ReferencesService& referencesService,
        const Auth::Session& session
    );

    void handle(
        int creatorId
    );
};