#include "type/IntType.h"
#include "exception/InvalidLiteralException.h"

#include "gmock/gmock.h"

using namespace testing;

class IntTypeTest : public Test
{
public:
    IntType intType;
};

TEST_F(IntTypeTest, toBytes0x12345678)
{
    intType.setVal(0x12345678);
    ASSERT_THAT(intType.toBytes(), Eq(std::vector<unsigned char>({0x78, 0x56, 0x34, 0x12})));
}

TEST_F(IntTypeTest, fromBytes0x12345678)
{
    intType.fromBytes({0x78, 0x56, 0x34, 0x12});
    ASSERT_THAT(intType.getVal(), Eq(0x12345678));
}

TEST_F(IntTypeTest, fromString)
{
    intType.fromString("123");
    ASSERT_THAT(intType.getVal(), Eq(123));
}

TEST_F(IntTypeTest, fromStringTooLarge)
{
    ASSERT_THROW(intType.fromString("2147483648"), InvalidLiteralException);
}

TEST_F(IntTypeTest, fromStringLargerThan64Bits)
{
    ASSERT_THROW(intType.fromString("999999999999999999999999999999"), InvalidLiteralException);
}

TEST_F(IntTypeTest, fromStringNotANumber)
{
    ASSERT_THROW(intType.fromString("0xf"), InvalidLiteralException);
}

TEST_F(IntTypeTest, toString)
{
    intType.setVal(456);
    ASSERT_THAT(intType.toString(), Eq("456"));
}
