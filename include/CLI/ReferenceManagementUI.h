// ReferenceManagementUI.h

#pragma once

#include "Service/ReferencesService.h"

#include "Auth/AuthService.h"
#include "CLI/BaseUI.h"
#include "CLI/UI/TerminalLayout.h"

class ReferenceManagementUI : public BaseUI
{
private:

    ReferencesService& referencesService;

    Auth::Session currentSession;

public:

    ReferenceManagementUI(
        ReferencesService& service,
        const Auth::Session& session
    );

    void run();

private:

    // =====================================================
    // GENRES
    // =====================================================

    void genresMenu();

    void addGenre();

    void renameGenre();

    void deleteGenre();

    // =====================================================
    // TARGET AUDIENCE
    // =====================================================

    void targetAudienceMenu();

    void addTargetAudience();

    void renameTargetAudience();

    void deleteTargetAudience();

    // =====================================================
    // NICHES
    // =====================================================

    void nichesMenu();

    void addNiche();

    void renameNiche();

    void deleteNiche();

    // =====================================================
    // PLATFORMS
    // =====================================================

    void platformsMenu();

    void addPlatform();

    void renamePlatform();

    void deletePlatform();

    
    // =====================================================
    // POSITIONS
    // =====================================================

    void positionsMenu();

    void addPosition();

    void renamePosition();

    void deletePosition();

    // =====================================================
    // HELPERS
    // =====================================================

    int takeChoice();

    void pause();
};