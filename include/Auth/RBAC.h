//This will have the functions of all the Role based Access control feature 
#pragma once

#include "AuthService.h"   // for Session 


class RBAC {

public:

    static bool canCreate(const Auth::Session& session);

    static bool canUpdate(const Auth::Session& session);

    static bool canDelete(const Auth::Session& session);

    static bool canView(const Auth::Session& session);

};