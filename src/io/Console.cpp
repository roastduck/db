#include <iostream>
#include "Console.h"

bool Console::addCommand(const std::string &cmd)
{
    buf += cmd;
    if (!buf.empty() && buf.back() == ';')
    {
        // Don't worry about the situation that a line ends with
        // `';` (half a string ending with a semicolon). Because
        // half a string is not a leagel token, it will go wrong
        // any way
        input.parse(buf);
        buf.clear();
        return true;
    } else
    {
        buf.push_back('\n');
        return false;
    }
}

void Console::mainLoop()
{
    std::cout << "Entering interactive mode" << std::endl;
    std::cout << "Enter \"exit\" to exit" << std::endl;
    std::string cmd;
    while (true)
    {
        std::cout << std::endl;
        std::cout << "SQL> ";
        std::getline(std::cin, cmd);
        trim(cmd);
        if (cmd == "exit")
        {
            std::cout << "Bye" << std::endl;
            break;
        }
        while (!addCommand(cmd))
        {
            std::cout << "---> ";
            std::getline(std::cin, cmd);
        }
    }
}

