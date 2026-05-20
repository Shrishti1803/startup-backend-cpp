#pragma once

#include "Models/Creator/Creator.h"
#include "CLI/UI/TerminalLayout.h"
#include <string>
#include "CLI/Creator/CreatorCoreUI.h"
#include "CLI/Creator/CommunityReachesUI.h"
#include "CLI/Creator/ProfileAccountUI.h"
#include "CLI/Creator/HPVAuditUI.h"
#include "CLI/Creator/StrategicNoteUI.h"
#include "CLI/Creator/VideoL10UI.h"
#include "CLI/Creator/VerdictUI.h"
#include "CLI/Creator/NicheUI.h"
#include "CLI/Creator/CreatorAudienceUI.h"
#include "Models/References/Platform.h"
#include "Service/ReferencesService.h"
#include "Auth/AuthService.h"

class CreatorRenderUI
{
private:

    ReferencesService&
        referencesService;

    const Auth::Session&
        currentSession;

public:

    CreatorRenderUI(
        ReferencesService& refService,
        const Auth::Session& session
    );

    void render(
        const Creator& data
    );

    void displayCompactHeader(
        const Creator& data
    );

    void printSectionHeader(
        const std::string& title
    );

    // ================= CORE =================

    void displayCreatorCore(
        const Creator& data
    );

    // ================= COMMUNITY =================

    void displayCommunity(
        const Creator& data
    );

    // ================= NICHES =================

    void displayNiches(
        const Creator& data
    );

    // ================= TARGET AUDIENCE =================

    void displayTargetAudience(
        const Creator& data
    );

    // ================= PROFILE ACCOUNTS =================
    
    void displayProfileAccounts(
        const Creator& data,
        const std::vector<Platform>& platforms
    );

    // ================= HPV AUDITS =================

    void displayHPVAudits(
        const Creator& data
    );

    // ================= STRATEGIC NOTES =================

    void displayStrategicNotes(
        const Creator& data
    );

    // ================= VIDEO L10 =================

    void displayVideos(
        const Creator& data
    );

    // ================= VERDICT =================

    void displayVerdict(
        const Creator& data
    );
};