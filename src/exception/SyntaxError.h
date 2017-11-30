#ifndef SYNTAX_ERROR_H_
#define SYNTAX_ERROR_H_

#include <stdexcept>

class SyntaxError : public std::runtime_error
{
public:
    SyntaxError() :
        std::runtime_error("Syntax error")
    {}
};

#endif // SYNTAX_ERROR_H_
