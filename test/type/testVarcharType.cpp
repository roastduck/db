#include "type/VarcharType.h"

#include "gmock/gmock.h"

using namespace testing;

class VarcharTypeTest : public Test
{
public:
    VarcharType varcharType;
};

TEST_F(VarcharTypeTest, toBytes12)
{
    varcharType.setVal("12");
    ASSERT_THAT(varcharType.toBytes(), Eq(std::vector<unsigned char>({0x31, 0x32})));
}

TEST_F(VarcharTypeTest, fromBytes12)
{
    varcharType.fromBytes({0x31, 0x32});
    ASSERT_THAT(varcharType.getVal(), Eq("12"));
}

TEST_F(VarcharTypeTest, fromString)
{
    varcharType.fromString("abcde");
    ASSERT_THAT(varcharType.getVal(), Eq("abcde"));
}

TEST_F(VarcharTypeTest, toString)
{
    varcharType.setVal("def");
    ASSERT_THAT(varcharType.toString(), Eq("def"));
}
