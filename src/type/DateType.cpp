#include <cassert>
#include <sstream>
#include "IntType.h"
#include "../exception/InvalidLiteralException.h"

std::vector<unsigned char> IntType::toBytes() const
{
    std::vector<unsigned char> ret(2);
    for (int i = 0; i < 2; i++)
        ret[i] = *((unsigned char*)(&val) + i);
    return ret;
}

void IntType::fromBytes(const std::vector<unsigned char> &bytes)
{
    assert(bytes.size() == 2);
    for (int i = 0; i < 2; i++)
        *((unsigned char*)(&val) + i) = bytes[i];
}

// TODO: 关于日期的I/O格式，作业要求的不同文件里有出入。我发邮件问了助教，等他回复再写
