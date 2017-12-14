#ifndef IDENTIFIER_TOO_LONG_EXCEPTION_H_
#define IDENTIFIER_TOO_LONG_EXCEPTION_H_

#include <string>
#include <stdexcept>
#include "../config.h"

class IdentifierTooLongException : public std::runtime_error
{
public:
    IdentifierTooLongException(const std::string &identifier)
        : std::runtime_error(
            "Identifier `" + identifier + "` is too long. The maximum length allowed is " +
            std::to_string(MAX_IDENTIFIER_LEN) + " characters"
        )
    {}
};

#endif // IDENTIFIER_TOO_LONG_EXCEPTION_H_
