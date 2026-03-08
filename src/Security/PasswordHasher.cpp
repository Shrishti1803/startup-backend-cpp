#include "Security/PasswordHasher.h"
#include <sodium.h>
#include <string>
#include <stdexcept>
#include "Utils/Logger.h"

//WELCOME TO THE POLICE OF THIS APPLICATION! <3

namespace Security{

    //this one will verify the password "plain" that the user enteres during login.. with the stored hashed password
    bool verifyPassword(const std::string& plain, const std::string& hash){
        return crypto_pwhash_str_verify(
            hash.c_str(), plain.c_str(), plain.size()
        ) == 0;
    }

    //This one hashes the password.. This is done only once for each Login .. Only for the First Login 
    std::string hashPassword(const std::string& password){
        char hash[crypto_pwhash_STRBYTES];

        if(crypto_pwhash_str(hash, password.c_str(), password.size(), 
                            crypto_pwhash_OPSLIMIT_INTERACTIVE, 
                            crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0){
                                auto auth_logger = Logger::auth();
                                auth_logger->info("Password hashing failed");
                                throw std::runtime_error("Password hashing failed!");
                                
                            }

        return std::string(hash);
    }
}