#ifndef CHECK_VIOLATED_EXCEPTION_H_
#define CHECK_VIOLATED_EXCEPTION_H_

#include <cassert>
#include <stdexcept>

class CheckViolatedException : public std::runtime_error
{
public:
    CheckViolatedException(const std::string &tb, const std::string &col)
        : std::runtime_error("CHECK constraint for " + tb + "(" + col + ") is violated")
    {}
};

#endif // CHECK_VIOLATED_EXCEPTION_H_
