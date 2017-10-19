#include "type/FloatType.h"

#include "gmock/gmock.h"

using namespace testing;

class FloatTypeTest : public Test
{
public:
    FloatType floatType;
};

TEST_F(FloatTypeTest, toBytes1)
{
    floatType.setVal(1.0);
    ASSERT_THAT(floatType.toBytes(), Eq(std::vector<unsigned char>({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x3f})));
}

TEST_F(FloatTypeTest, fromBytes1)
{
    floatType.fromBytes({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x3f});
    ASSERT_THAT(floatType.getVal(), Eq(1.0));
}

TEST_F(FloatTypeTest, fromString)
{
    floatType.fromString("1.23");
    ASSERT_THAT(floatType.getVal(), Eq(1.23));
}

TEST_F(FloatTypeTest, toString)
{
    floatType.setVal(4.56);
    ASSERT_THAT(floatType.toString(), Eq("4.56"));
}
