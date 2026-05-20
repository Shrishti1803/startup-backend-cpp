#pragma once

#include <string>

namespace Style {

    inline std::string reset() {
        return "\033[0m";
    }

    inline std::string bold() {
        return "\033[1m";
    }

    inline std::string red() {
        return "\033[31m";
    }

    inline std::string green() {
        return "\033[32m";
    }

    inline std::string yellow() {
        return "\033[33m";
    }

    inline std::string cyan() {
        return "\033[36m";
    }
}