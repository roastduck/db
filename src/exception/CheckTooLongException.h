#ifndef CHECK_TOO_LONG_EXCEPTION_H_
#define CHECK_TOO_LONG_EXCEPTION_H_

#include <string>
#include <stdexcept>
#include "../config.h"

class CheckTooLongException : public std::runtime_error
{
public:
    CheckTooLongException() :
        std::runtime_error(
            "CHECK candidate longer than " + std::to_string(MAX_CHECK_LEN) +
            " characters is currently not supported"
        )
    {}
};

#endif // CHECK_TOO_LONG_EXCEPTION_H_
