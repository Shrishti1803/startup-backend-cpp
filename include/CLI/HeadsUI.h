#pragma once

#include "CLI/BaseUI.h"

#include "Service/BrandService.h"
#include "Service/ReferencesService.h"
#include "Auth/AuthService.h"
#include "CLI/BrandRenderUI.h"
#include "CLI/UI/TerminalLayout.h"
class HeadsUI : public BaseUI
{
private:

    BrandService& brandService;

    const Auth::Session& currentSession;

    ReferencesService& referencesService;

public:

    HeadsUI(
        BrandService& service,
        ReferencesService& refService,
        const Auth::Session& session
    );
    
    void handle(int brandId);
};