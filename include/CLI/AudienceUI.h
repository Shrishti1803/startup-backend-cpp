#pragma once

#include "CLI/BaseUI.h"

#include "Service/BrandService.h"
#include "Service/ReferencesService.h"

#include "Auth/AuthService.h"
#include "CLI/UI/TerminalLayout.h"
class AudienceUI : public BaseUI
{
private:

    BrandService& brandService;

    ReferencesService& referencesService;

    const Auth::Session& currentSession;

public:

    AudienceUI(
        BrandService& service,
        ReferencesService& refService,
        const Auth::Session& session
    );

    void handle(int brandId);
};