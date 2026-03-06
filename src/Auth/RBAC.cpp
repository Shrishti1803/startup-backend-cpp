#include "RBAC.h"

// Everyone can create
bool canCreate(const Session& session)
{
    if(session.role == "DBA" || 
       session.role == "ADMIN" || 
       session.role == "USER")
        return true;

    return false;
}


// Everyone can update
bool canUpdate(const Session& session)
{
    if(session.role == "DBA" || 
       session.role == "ADMIN" || 
       session.role == "USER")
        return true;

    return false;
}


// Only DBA and ADMIN can delete
bool canDelete(const Session& session)
{
    if(session.role == "DBA" || 
       session.role == "ADMIN")
        return true;

    return false;
}


// Everyone can view
bool canView(const Session& session)
{
    if(session.role == "DBA" || 
       session.role == "ADMIN" || 
       session.role == "USER")
        return true;

    return false;
}