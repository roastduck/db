#ifndef MULTIPLE_PRIMARY_EXCEPTION_H_
#define MULTIPLE_PRIMARY_EXCEPTION_H_

#include <stdexcept>

class MultiplePrimaryException : public std::runtime_error
{
public:
    MultiplePrimaryException()
        : std::runtime_error("There can only be at most one primary index in a table")
    {}
};

#endif // MULTIPLE_PRIMARY_EXCEPTION_H_
