#include "type/CharType.h"
#include "exception/InvalidLiteralException.h"

#include "gmock/gmock.h"

using namespace testing;

class CharTypeTest : public Test
{
public:
    CharType charType;

    CharTypeTest() : charType(5) {}
};

TEST_F(CharTypeTest, toBytes12)
{
    charType.setVal("12");
    ASSERT_THAT(charType.toBytes(), Eq(std::vector<unsigned char>({0x31, 0x32, 0x00, 0x00, 0x00})));
}

TEST_F(CharTypeTest, fromBytes12)
{
    charType.fromBytes({0x31, 0x32, 0x00, 0x00, 0x00});
    ASSERT_THAT(charType.getVal(), Eq("12"));
}

TEST_F(CharTypeTest, fromString)
{
    charType.fromString("abcde");
    ASSERT_THAT(charType.getVal(), Eq("abcde"));
}

TEST_F(CharTypeTest, fromStringTooLong)
{
    ASSERT_THROW(charType.fromString("abcdef"), InvalidLiteralException);
}

TEST_F(CharTypeTest, toString)
{
    charType.setVal("def");
    ASSERT_THAT(charType.toString(), Eq("def"));
}
