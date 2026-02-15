#include "Security/PasswordHasher.h"
#include <sodium.h>
#include <string>
#include <stdexcept>
#include "Utils/Logger.h"

namespace Security{

    bool verifyPassword(const std::string& plain, const std::string& hash){
        return crypto_pwhash_str_verify(
            hash.c_str(), plain.c_str(), plain.size()
        ) == 0;
    }

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