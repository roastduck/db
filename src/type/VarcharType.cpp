#include <cassert>
#include <string>
#include "VarcharType.h"

std::vector<unsigned char> VarcharType::toBytes() const
{
    return std::vector<unsigned char>(val.begin(), val.end());
}

void VarcharType::fromBytes(const std::vector<unsigned char> &bytes)
{
    val = std::string(bytes.begin(), bytes.end());
}

