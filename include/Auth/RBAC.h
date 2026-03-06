//This will have the functions of all the Role based Access control feature 
#pragma once

#include <string>
#include "AuthService.h" 

bool canCreateBrand(const Session& session);
bool canUpdateBrand(const Session& session);
bool canDeleteBrand(const Session& session);
bool canViewBrand(const Session& session);