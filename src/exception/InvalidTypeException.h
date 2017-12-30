#ifndef INVALID_TYPE_EXCEPTION_H_
#define INVALID_TYPE_EXCEPTION_H_

#include <string>
#include <stdexcept>
#include "../Type.h"

class InvalidTypeException : public std::runtime_error
{
public:
    InvalidTypeException(Type::TypeID type, const std::string &what)
        : std::runtime_error("Invalid type " + Type::getName(type) + " for " + what)
    {}
};

#endif // INVALID_TYPE_EXCEPTION_H_
