#pragma once

#include "CLI/BaseUI.h"

#include "Service/BrandService.h"

#include "Auth/AuthService.h"
#include "CLI/BrandRenderUI.h"
#include "CLI/UI/TerminalLayout.h"
class StandardUI : public BaseUI
{
private:

    BrandService& brandService;

    const Auth::Session& currentSession;

public:

    StandardUI(
        BrandService& service,
        const Auth::Session& session
    );

    void handle(int brandId);
};