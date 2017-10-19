#include <cassert>
#include <string>
#include "CharType.h"
#include "../exception/InvalidLiteralException.h"

std::vector<unsigned char> CharType::toBytes() const
{
    std::vector<unsigned char> ret(length, 0);
    std::copy(val.begin(), val.end(), ret.begin());
    return ret;
}

void CharType::fromBytes(const std::vector<unsigned char> &bytes)
{
    assert(int(bytes.size()) == length);
    val.clear();
    for (int i = 0; i < length; i++)
    {
        if (!bytes[i]) break;
        val.push_back(bytes[i]);
    }
}

void CharType::fromString(const std::string &s)
{
    if (int(s.length()) > length)
        throw InvalidLiteralException(s, CHAR);
    val = s;
}

