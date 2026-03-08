//This will have the functions of all the Role based Access control feature 
#pragma once

#include "AuthService.h"   // for Session 


class RBAC {

public:

    static bool canCreate(const Session& session);

    static bool canUpdate(const Session& session);

    static bool canDelete(const Session& session);

    static bool canView(const Session& session);

};