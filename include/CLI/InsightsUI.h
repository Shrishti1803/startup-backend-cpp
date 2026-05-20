#pragma once

#include "CLI/BaseUI.h"

#include "Service/BrandService.h"

#include "Auth/AuthService.h"
#include "CLI/BrandRenderUI.h"
#include "CLI/UI/TerminalLayout.h"
class InsightsUI : public BaseUI
{
private:

    BrandService& brandService;

    const Auth::Session& currentSession;

public:

    InsightsUI(
        BrandService& service,
        const Auth::Session& session
    );

    void handle(int brandId);
};