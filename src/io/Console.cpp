#include "Console.h"

void Console::addCommand(const std::string &cmd)
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
    } else
        buf.push_back('\n');
}

