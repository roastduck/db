#include <cerrno>
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
    val = strtol(s.c_str(), &end, 10);
    if (errno || s.empty() || *end != '\0')
        throw InvalidLiteralException(s, INT);
}

