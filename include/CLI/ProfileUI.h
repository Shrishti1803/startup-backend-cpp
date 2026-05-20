#pragma once

#include "Auth/AuthService.h"
#include "Db/DbManager.h"
#include "CLI/UI/TerminalLayout.h"
#include "CLI/BaseUI.h"
class ProfileUI : public BaseUI {

    private:

        DbManager& db;

        const Auth::Session& currentSession;

    public:

        ProfileUI(
            DbManager& db,
            const Auth::Session& session
        );

        void run();

    private:

        void showProfile();

        void changePasswordFlow();

        int takeChoice();

        void pause();
};