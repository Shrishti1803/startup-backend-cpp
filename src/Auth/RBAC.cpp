/*
This layer is implemented according to the startup requirements 
The CURD operation is constant for all the roles except DELETE not permitted for the "USER" role
The Database has these implemented accordingly
*/

#include "Auth/RBAC.h"


bool RBAC::canCreate(const Auth::Session& session)
{
    return session.role == "DBA"
        || session.role == "ADMIN"
        || session.role == "USER";
}


bool RBAC::canUpdate(const Auth::Session& session)
{
    return session.role == "DBA"
        || session.role == "ADMIN"
        || session.role == "USER";
}


bool RBAC::canDelete(const Auth::Session& session)
{
    return session.role == "DBA"
        || session.role == "ADMIN";
}


bool RBAC::canView(const Auth::Session& session)
{
    return session.role == "DBA"
        || session.role == "ADMIN"
        || session.role == "USER";
}