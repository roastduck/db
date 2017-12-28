#include "type/DateType.h"
#include "exception/InvalidLiteralException.h"

#include "gmock/gmock.h"

using namespace testing;

class DateTypeTest : public Test
{
public:
    DateType dateType;
};

TEST_F(DateTypeTest, toBytes0x123456789)
{
    dateType.setVal(0x123456789ll);
    ASSERT_THAT(dateType.toBytes(), Eq(std::vector<unsigned char>({0x89, 0x67, 0x45, 0x23, 0x01, 0x00, 0x00, 0x00})));
}

TEST_F(DateTypeTest, fromBytes0x123456789)
{
    dateType.fromBytes({0x89, 0x67, 0x45, 0x23, 0x01, 0x00, 0x00, 0x00});
    ASSERT_THAT(dateType.getVal(), Eq(0x123456789ll));
}

TEST_F(DateTypeTest, fromString)
{
    dateType.fromString("2010-2-15");
    int year, month, day;
    DateType::parseDate(dateType.getVal(), year, month, day);
    ASSERT_THAT(year, Eq(2010));
    ASSERT_THAT(month, Eq(2));
    ASSERT_THAT(day, Eq(15));
}

TEST_F(DateTypeTest, fromStringWithLeadingZero)
{
    dateType.fromString("2010-02-15");
    int year, month, day;
    DateType::parseDate(dateType.getVal(), year, month, day);
    ASSERT_THAT(year, Eq(2010));
    ASSERT_THAT(month, Eq(2));
    ASSERT_THAT(day, Eq(15));
}

TEST_F(DateTypeTest, fromStringTooEarly)
{
    ASSERT_THROW(dateType.fromString("1969-12-31"), InvalidLiteralException);
}

TEST_F(DateTypeTest, fromStringTooLate)
{
    ASSERT_THROW(dateType.fromString("2100-1-1"), InvalidLiteralException);
}

TEST_F(DateTypeTest, fromStringNextMonth)
{
    ASSERT_THROW(dateType.fromString("2017-2-29"), InvalidLiteralException);
}

TEST_F(DateTypeTest, fromStringLeapYear)
{
    dateType.fromString("2012-2-29");
    int year, month, day;
    DateType::parseDate(dateType.getVal(), year, month, day);
    ASSERT_THAT(year, Eq(2012));
    ASSERT_THAT(month, Eq(2));
    ASSERT_THAT(day, Eq(29));
}

TEST_F(DateTypeTest, fromStringTailing)
{
    ASSERT_THROW(dateType.fromString("2010-02-15-"), InvalidLiteralException);
}

TEST_F(DateTypeTest, fromStringNotDate)
{
    ASSERT_THROW(dateType.fromString("abc"), InvalidLiteralException);
}

TEST_F(DateTypeTest, toString)
{
    dateType.fromString("1970-01-01");
    ASSERT_THAT(dateType.toString(), Eq("1970-01-01"));
}
