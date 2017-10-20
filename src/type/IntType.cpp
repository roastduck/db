#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cassert>
#include <sstream>
#include "IntType.h"
#include "../exception/InvalidLiteralException.h"

std::vector<unsigned char> IntType::toBytes() const
{
    std::vector<unsigned char> ret(4);
    for (int i = 0; i < 4; i++)
        ret[i] = *((unsigned char*)(&val) + i);
    return ret;
}

void IntType::fromBytes(const std::vector<unsigned char> &bytes)
{
    assert(bytes.size() == 4);
    for (int i = 0; i < 4; i++)
        *((unsigned char*)(&val) + i) = bytes[i];
}

std::string IntType::toString() const
{
    std::ostringstream os;
    os << val;
    return os.str();
}

void IntType::fromString(const std::string &s)
{
    char *end;
    errno = 0;
    long l = strtol(s.c_str(), &end, 10); // long is 64-bit in 64-bit machine
    if (errno || s.empty() || *end != '\0' || l > INT_MAX || l < INT_MIN)
        throw InvalidLiteralException(s, INT);
    val = l;
}

bool operator==(const IntType &t1, const IntType &t2)
{
    return t1.val == t2.val;
}

bool operator!=(const IntType &t1, const IntType &t2)
{
    return t1.val != t2.val;
}

