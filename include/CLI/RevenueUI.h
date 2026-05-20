#pragma once

#include <string>

#include "Service/BrandService.h"
#include "Auth/AuthService.h"
#include "CLI/BaseUI.h"
#include "CLI/BrandRenderUI.h"
#include "CLI/UI/TerminalLayout.h"
class RevenueUI : public BaseUI
{
private:

    BrandService& brandService;

    const Auth::Session& currentSession;

public:

    RevenueUI(
        BrandService& service,
        const Auth::Session& session
    );

    void handle(int brandId);
};