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

std::unique_ptr<Type> Type::newFromCopy(const std::unique_ptr<Type> &ori)
{
    if (ori == nullptr)
        return nullptr;
    std::unique_ptr<Type> ret = newType(ori->getTypeID(), ori->getLength());
    switch (ori->getTypeID())
    {
    case INT:
        dynamic_cast<IntType*>(ret.get())->setVal(dynamic_cast<IntType*>(ori.get())->getVal());
        break;
    case FLOAT:
        dynamic_cast<FloatType*>(ret.get())->setVal(dynamic_cast<FloatType*>(ori.get())->getVal());
        break;
    case DATE:
        dynamic_cast<DateType*>(ret.get())->setVal(dynamic_cast<DateType*>(ori.get())->getVal());
        break;
    case CHAR:
        dynamic_cast<CharType*>(ret.get())->setVal(dynamic_cast<CharType*>(ori.get())->getVal());
        break;
    default:
        assert(false);
    }
    return ret;
}

bool Type::less(const std::unique_ptr<Type> &lhs, const std::unique_ptr<Type> &rhs)
{
    if (lhs == nullptr && rhs == nullptr) return false;
    if (lhs == nullptr) return true;
    if (rhs == nullptr) return false;
    return *lhs < *rhs;
}

bool Type::equal(const std::unique_ptr<Type> &lhs, const std::unique_ptr<Type> &rhs)
{
    if (lhs == nullptr && rhs == nullptr) return true;
    if (lhs == nullptr || rhs == nullptr) return false;
    return *lhs == *rhs;
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

