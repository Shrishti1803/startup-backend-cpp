/*This file is a class for the Transaction level work (commit & rollback)
RAII application so that the "conn" pointer don't need to be constructed and deconstructed manually 
The constructor and deconstructor would handle that automatically 
This will prevent memory leaks*/


#pragma once

#include <cppconn/connection.h>

class TransactionGuard {

private:
    sql::Connection* conn;
    bool committed;

public:

    TransactionGuard(sql::Connection* connection)
        : conn(connection), committed(false)
    {
        if (conn)
            conn->setAutoCommit(false);
    }

    void commit()
    {
        if (conn) {
            conn->commit();
            committed = true;
        }
    }

    ~TransactionGuard()
    {
        if (!conn) return;

        if (!committed) {
            conn->rollback();
        }

        conn->setAutoCommit(true);
    }
};