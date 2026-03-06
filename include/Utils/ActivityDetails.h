//This file will have a function which will be called after EVERY CURD OPERATION 
//WHY?
// SO THAT ALL THE DETAILS OF WHO MADE THE CHANGES, WHEN THE CHANGED WERE MADE, WHAT CHANGES WERE MADE , WHICH TABLE AND WHICH RECORD OF THAT TABLE WAS CHANGED
//ALL THESE DETAILS WILL BE DIRECTLY STORED IN THE ACTIVITY_LOG TABLE IN THE DB 
//Yk... Cannot afford to Miss out on the Important info on who MADE THE CHANGES... 
//Will be something like : ActivityLog(Session, actionType, tablename, recordid)
// Now before you wonder... How on earth you will extract the table name...Well.. MAake classes for eeach Table in the Db/Required_file.h AND use those classes here !
//The recordid will easily be extracted after that 
//When it comes to actionType.. Make another class in the DbManager.h 

#pragma once

#include <string>
#include <cppconn/connection.h>

class ActivityDetails
{
public:

    static void logActivity(
        sql::Connection* con,
        int user_id,
        const std::string& action,
        const std::string& entity,
        int entity_id,
        const std::string& field_name,
        const std::string& old_value
    );

};