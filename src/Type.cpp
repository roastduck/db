#include <cassert>
#include "Type.h"
#include "type/IntType.h"
#include "type/FloatType.h"
#include "type/DateType.h"
#include "type/CharType.h"

std::unique_ptr<Type> Type::newType(Type::TypeID typeID, int length)
{
    switch (typeID)
    {
    case INT:
        return std::unique_ptr<Type>(new IntType(length));
    case FLOAT:
        return std::unique_ptr<Type>(new FloatType(length));
    case DATE:
        return std::unique_ptr<Type>(new DateType(length));
    case CHAR:
        return std::unique_ptr<Type>(new CharType(length));
    default:
        assert(false);
    }
}

std::unique_ptr<Type> Type::newFromLiteral(const std::string &literal, TypeID typeID, int length)
{
    auto ret = newType(typeID, length);
    ret->fromString(literal);
    return ret;
}

std::unique_ptr<Type> Type::newFromBytes(const std::vector<unsigned char> &bytes, TypeID typeID, int length)
{
    auto ret = newType(typeID, length);
    ret->fromBytes(bytes);
    return ret;
}

#define GEN_OP(op) \
    bool operator op (const Type &t1, const Type &t2) \
    { \
        assert(t1.getTypeID() == t2.getTypeID()); \
        switch (t1.getTypeID()) \
        { \
        case Type::INT: \
            return (IntType&)t1 op (IntType&)t2; \
        case Type::FLOAT: \
            return (FloatType&)t1 op (FloatType&)t2; \
        case Type::DATE: \
            return (DateType&)t1 op (DateType&)t2; \
        case Type::CHAR: \
            return (CharType&)t1 op (CharType&)t2; \
        default: \
            assert(false); \
        } \
    } \

GEN_OP(==)
GEN_OP(!=)
GEN_OP(<)
GEN_OP(<=)
GEN_OP(>)
GEN_OP(>=)

#undef GEN_OP

