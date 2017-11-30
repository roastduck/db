#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <string>
#include "Input.h"

class Console
{
private:
    Input &input;
    std::string buf;

public:
    Console(Input &_input) : input(_input) {}

    void addCommand(const std::string &cmd);
};

#endif // CONSOLE_H_
