#ifndef PASSWORD_HASHER_H
#define PASSWORD_HASHER_H

#include<string>

namespace Security{
    bool verifyPassword(const std::string& plain, 
        const std::string& hash
    );

    std::string hashPassword(const std::string& password);
};

#endif