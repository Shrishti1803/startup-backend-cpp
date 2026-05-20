#pragma once

#include <string>
#include "CLI/UI/TerminalLayout.h"
class BaseUI
{
protected:

    int takeChoice();

    void pause();

    void clearScreen();

    static bool isValidInteger(
        const std::string& s
    );

    static bool isValidDouble(
        const std::string& s
    );
};