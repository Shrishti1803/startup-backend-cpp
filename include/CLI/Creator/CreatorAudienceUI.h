#pragma once

#include "CLI/BaseUI.h"

#include "Service/CreatorService.h"
#include "Service/ReferencesService.h"
#include "CLI/UI/TerminalLayout.h"
#include "Auth/AuthService.h"
#include "CLI/Creator/CreatorRenderUI.h"

class CreatorAudienceUI : public BaseUI
{
private:

    CreatorService& creatorService;

    ReferencesService& referencesService;

    const Auth::Session& currentSession;

public:

    CreatorAudienceUI(
        CreatorService& service,
        ReferencesService& refService,
        const Auth::Session& session
    );

    void handle(int creatorId);
};