#include "RBAC.h"


bool RBAC::canCreate(const Session& session)
{
    return session.role == "DBA"
        || session.role == "ADMIN"
        || session.role == "USER";
}


bool RBAC::canUpdate(const Session& session)
{
    return session.role == "DBA"
        || session.role == "ADMIN"
        || session.role == "USER";
}


bool RBAC::canDelete(const Session& session)
{
    return session.role == "DBA"
        || session.role == "ADMIN";
}


bool RBAC::canView(const Session& session)
{
    return session.role == "DBA"
        || session.role == "ADMIN"
        || session.role == "USER";
}