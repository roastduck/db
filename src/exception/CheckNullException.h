#ifndef CHECK_NULL_EXCEPTION_H_
#define CHECK_NULL_EXCEPTION_H_

#include <string>
#include <stdexcept>

class CheckNullException : public std::runtime_error
{
public:
    CheckNullException() :
        std::runtime_error("NULL in a CHECK constraint is currently not supported")
    {}
};

#endif // CHECK_NULL_EXCEPTION_H_
