#ifndef NO_DB_IN_USE_EXCEPTION_H_
#define NO_DB_IN_USE_EXCEPTION_H_

#include <cassert>
#include <stdexcept>

class NoDBInUseException : public std::runtime_error
{
public:
    NoDBInUseException()
        : std::runtime_error("You should use a DB before performing an operation")
    {}
};

#endif // NO_DB_IN_USE_EXCEPTION_H_
