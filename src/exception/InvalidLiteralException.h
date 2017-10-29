#ifndef INVALID_LITERAL_EXCEPTION_H_
#define INVALID_LITERAL_EXCEPTION_H_

#include <cassert>
#include <stdexcept>
#include "../Type.h"

/** Cannot convert a literal to a type
 */
class InvalidLiteralException : public std::runtime_error
{
private:
    std::string toString(Type::TypeID typeId)
    {
        switch (typeId)
        {
        case Type::INT:
            return "INT";
        case Type::FLOAT:
            return "FLOAT";
        case Type::DATE:
            return "DATE";
        case Type::CHAR:
            return "CHAR";
        default:
            assert(false);
        }
    }

public:
    InvalidLiteralException(const std::string &literal, Type::TypeID typeID)
        : std::runtime_error("Cannot convert \"" + literal + "\" to " + toString(typeID)) {}
};

#endif // INVALID_LITERAL_EXCEPTION_H_
