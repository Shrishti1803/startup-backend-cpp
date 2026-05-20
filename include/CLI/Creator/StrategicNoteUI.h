// StrategicNoteUI.h

#pragma once

#include "CLI/BaseUI.h"

#include "Service/CreatorService.h"
#include "CLI/UI/TerminalLayout.h"
#include "Auth/AuthService.h"
#include "CLI/Creator/CreatorRenderUI.h"
#include "Service/ReferencesService.h"
class StrategicNoteUI : public BaseUI
{
private:

    CreatorService& creatorService;
    ReferencesService& referencesService;
    Auth::Session currentSession;

public:

    StrategicNoteUI(
        CreatorService& service,
        ReferencesService& referenceService,
        const Auth::Session& session
    );

    void handle(
        int creatorId
    );
};