#pragma once

#include "CLI/BaseUI.h"

#include "Service/CreatorService.h"
#include "CLI/UI/TerminalLayout.h"
#include "Auth/AuthService.h"
#include "CLI/Creator/CreatorRenderUI.h"
#include "Service/ReferencesService.h"
class VerdictUI : public BaseUI
{
private:

    CreatorService& creatorService;
    ReferencesService& referencesService;
    Auth::Session currentSession;

public:

    VerdictUI(
        CreatorService& service,
        ReferencesService& referencesService,
        const Auth::Session& session
    );

    void handle(int creatorId);
};