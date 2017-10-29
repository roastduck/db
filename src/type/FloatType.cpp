#include <cassert>
#include <sstream>
#include "FloatType.h"

std::vector<unsigned char> FloatType::toBytes() const
{
    std::vector<unsigned char> ret(8);
    for (int i = 0; i < 8; i++)
        ret[i] = *((unsigned char*)(&val) + i);
    return ret;
}

void FloatType::fromBytes(const std::vector<unsigned char> &bytes)
{
    assert(bytes.size() == 8);
    for (int i = 0; i < 8; i++)
        *((unsigned char*)(&val) + i) = bytes[i];
}

std::string FloatType::toString() const
{
    std::ostringstream os;
    os << val;
    return os.str();
}

void FloatType::fromString(const std::string &s)
{
    std::istringstream is(s);
    is >> val;
}

#define GEN_OP(op) \
    bool operator op (const FloatType &t1, const FloatType &t2) \
    { \
        return t1.val op t2.val; \
    } \

GEN_OP(==)
GEN_OP(!=)
GEN_OP(<)
GEN_OP(<=)
GEN_OP(>)
GEN_OP(>=)

#undef GEN_OP
