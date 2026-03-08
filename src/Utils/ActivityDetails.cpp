#include "Utils/ActivityDetails.h"

#include <memory>
#include <cppconn/prepared_statement.h>

void ActivityDetails::logActivity(
    sql::Connection* con,
    int user_id,
    const std::string& action,
    const std::string& entity,
    int entity_id,
    const std::string& field_name,
    const std::string& old_value
)
{
    std::unique_ptr<sql::PreparedStatement> pstmt(
        con->prepareStatement(
            "INSERT INTO ACTIVITY_LOG "
            "(user_id, action, entity, entity_id, field_name, old_value) "
            "VALUES (?, ?, ?, ?, ?, ?)"
        )
    );

    pstmt->setInt(1, user_id);
    pstmt->setString(2, action);
    pstmt->setString(3, entity);
    pstmt->setInt(4, entity_id);
    pstmt->setString(5, field_name);
    pstmt->setString(6, old_value);

    pstmt->executeUpdate();
}