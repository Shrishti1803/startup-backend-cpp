#pragma once

#include "CLI/BaseUI.h"

#include "Service/BrandService.h"
#include "CLI/BrandRenderUI.h"
#include "CLI/UI/TerminalLayout.h"
class PastInfluencerCampUI : public BaseUI
{
private:

    BrandService& brandService;

    const Auth::Session& currentSession;

public:

    PastInfluencerCampUI(
        BrandService& service,
        const Auth::Session& session
    );

    void handle(int brandId);
};