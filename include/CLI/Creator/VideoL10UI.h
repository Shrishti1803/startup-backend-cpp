// VideoL10UI.h

#pragma once

#include "CLI/BaseUI.h"

#include "Service/CreatorService.h"
#include "CLI/UI/TerminalLayout.h"
#include "Auth/AuthService.h"
#include "CLI/Creator/CreatorRenderUI.h"
#include "Service/ReferencesService.h"

class VideoL10UI : public BaseUI
{
private:

    CreatorService& creatorService;
    ReferencesService& referencesService;
    Auth::Session currentSession;

public:

    VideoL10UI(
        CreatorService& service,
        ReferencesService& referencesService,
        const Auth::Session& session
    );

    void handle(
        int creatorId
    );
};