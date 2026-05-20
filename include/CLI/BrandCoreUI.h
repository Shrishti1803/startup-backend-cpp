#pragma once

#include "CLI/BaseUI.h"
#include "CLI/UI/TerminalLayout.h"
#include "Service/BrandService.h"

#include "Auth/AuthService.h"
#include "CLI/BrandRenderUI.h"

class BrandCoreUI : public BaseUI
{
private:

    BrandService& brandService;

    const Auth::Session& currentSession;

public:

    BrandCoreUI(
        BrandService& service,
        const Auth::Session& session
    );

    void handle(int brandId);
};