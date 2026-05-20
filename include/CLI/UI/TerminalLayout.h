#pragma once

#include <iostream>
#include <string>

#include "CLI/UI/TerminalStyle.h"

namespace UI {

    inline void divider() {
        std::cout << "\n==================================================\n";
    }

    inline void title(const std::string& text) {
        divider();

        std::cout
            << Style::bold()
            << Style::cyan()
            << "                 " << text
            << Style::reset()
            << "\n";

        divider();
    }

    inline void success(const std::string& text) {
        std::cout
            << Style::green()
            << "\n✓ " << text
            << Style::reset()
            << "\n";
    }

    inline void error(const std::string& text) {
        std::cout
            << Style::red()
            << "\n✗ " << text
            << Style::reset()
            << "\n";
    }

    inline void warning(const std::string& text) {
        std::cout
            << Style::yellow()
            << "\n! " << text
            << Style::reset()
            << "\n";
    }
}