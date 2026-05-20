#include "CLI/BaseUI.h"

#include <iostream>
#include <limits>
#include <cctype>

int BaseUI::takeChoice()
{
    int choice;

    while(!(std::cin >> choice))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout << "Enter the option number: ";
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    return choice;
}

void BaseUI::pause()
{
    std::cout << "\nPress Enter to continue...";

    std::cin.get();
}

void BaseUI::clearScreen()
{
    std::cout << std::string(40, '\n');
}

bool BaseUI::isValidInteger(
    const std::string& s
)
{
    if(s.empty())
        return false;

    for(char c : s)
    {
        if(!isdigit(c) && c != '-')
            return false;
    }

    return true;
}

bool BaseUI::isValidDouble(
    const std::string& s
)
{
    if(s.empty())
        return false;

    bool dotSeen = false;

    for(char c : s)
    {
        if(c == '.')
        {
            if(dotSeen)
                return false;

            dotSeen = true;
        }
        else if(!isdigit(c) && c != '-')
        {
            return false;
        }
    }

    return true;
}