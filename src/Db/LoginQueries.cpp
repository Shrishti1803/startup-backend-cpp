#include "Db/LoginQueries.h"

#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>


//This function just inserts the email and pass hash in the LOGIN table.... true if successful & false if not 
bool InsertLogin(sql::Connection* con,
                int user_id, 
                const std::string& password_hash){
            
            try{
                std::unique_ptr<sql::PreparedStatement> pstmt(
                    con->prepareStatement("INSERT into LOGIN(user_id, password_hash) VALUES (?,?)")
                );
                
                pstmt->setInt(1,user_id);
                pstmt->setString(2,password_hash);

                int rows = pstmt->executeUpdate(); //this works with INSERT sql queries

                return (rows == 1);
            }
            catch(const sql::SQLException){
                return false;
            }

}


/* this function check if the email entered is present in the ADMIN table
if email not present -> user access denied
if email present -> check if login_id is there or not.. If not there, then it is the 1st login...
                 -> if login_id found in ADMIN <=> existing user logging in again

*/
bool AdminQueryByEmail(sql::Connection* con, 
    const std::string& email,
    AdminRecord& outAdmin){
        try{
            std::unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement(
                "SELECT user_id, email, role "
                "FROM ADMIN WHERE email = ? LIMIT 1"
            )
            
            );

            pstmt->setString(1,email);
  
            std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

            if(!res->next()){
                return false;
            }

                outAdmin.user_id = res->getInt("user_id");
                outAdmin.email = res->getString("email");
                outAdmin.role = res->getString("role");


            return true;
        }
        catch(const sql::SQLException&){
            return false;
        }
}



bool GetPasswordHash(sql::Connection* con, int user_id,
    std::string& outHash){
        try{
            std::unique_ptr<sql::PreparedStatement> pstmt(
                con->prepareStatement(
                    "SELECT password_hash from LOGIN where user_id = ? LIMIT 1"
                )
            );
            pstmt->setInt(1,user_id);

            auto res = pstmt->executeQuery();

            if(!res->next()){
                return false;
            }
            
            outHash = res->getString("password_hash");
            return true;

        }

        catch(const sql::SQLException&){
            return false;
        }

}


/*
This function will check if login details are already existing in the LOGIN table
if yes... Will return true 
if no... will return false
*/
bool LoginExistForUser(sql::Connection* con, int user_id){
    try{
        std::unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement(
                "Select login_id "
                "FROM LOGIN where user_id = ? LIMIT 1"
            )
        );

        pstmt->setInt(1,user_id);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        return res->next();
    
    }

    catch(const sql::SQLException&){
        return false;
    }
}