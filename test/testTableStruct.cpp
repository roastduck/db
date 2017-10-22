#include "Type.h"
#include "Column.h"
#include "RawRecord.h"
#include "TableStruct.h"
#include "type/CharType.h"

#include "gmock/gmock.h"

using namespace testing;

class TableStructTest : public Test
{
public:
    TableStruct table;

    TableStructTest()
        : table({
            (Column){ Type::INT, 0, "int", false, {} }, // fixed 4
            (Column){ Type::VARCHAR, 0, "varchar1", false, {} },
            (Column){ Type::CHAR, 2, "xxORyy", true, { "xx", "yy" } }, // fixed 2, not null
            (Column){ Type::VARCHAR, 0, "varchar2", false, {} }
          })
        {}
};

TEST_F(TableStructTest, nullableCount)
{
    ASSERT_THAT(table.getNullableCount(), Eq(3));
}

TEST_F(TableStructTest, fixedSize)
{
    ASSERT_THAT(table.getFixedLengthSize(), Eq(6));
}

TEST_F(TableStructTest, varCount)
{
    ASSERT_THAT(table.getVarLengthCount(), Eq(2));
}

TEST_F(TableStructTest, getValue)
{
    RawRecord raw;
    raw.nullColumns = {false, true, false};
    raw.fixedLengths = {0x01, 0x02, 0x03, 0x04, 'x', 'x'};
    raw.varLengths = {{}, {0x34, 0x35, 0x36} };
    ASSERT_TRUE(*table.getValue(raw, "int") == *Type::newFromBytes({0x01, 0x02, 0x03, 0x04}, Type::INT));
    ASSERT_TRUE(table.getValue(raw, "varchar1") == nullptr);
    ASSERT_TRUE(*table.getValue(raw, "xxORyy") == *Type::newFromBytes({'x', 'x'}, Type::CHAR, 2));
    ASSERT_TRUE(*table.getValue(raw, "varchar2") == *Type::newFromBytes({0x34, 0x35, 0x36}, Type::VARCHAR));
}

TEST_F(TableStructTest, setValue)
{
    RawRecord raw;
    table.setValue(raw, "int", Type::newFromBytes({0x01, 0x02, 0x03, 0x04}, Type::INT));
    table.setValue(raw, "varchar1", nullptr);
    table.setValue(raw, "xxORyy", Type::newFromBytes({'x', 'x'}, Type::CHAR, 2));
    table.setValue(raw, "varchar2", Type::newFromBytes({0x34, 0x35, 0x36}, Type::VARCHAR));
    RawRecord correct;
    correct.nullColumns = {false, true, false};
    correct.fixedLengths = {0x01, 0x02, 0x03, 0x04, 'x', 'x'};
    correct.varLengths = {{}, {0x34, 0x35, 0x36} };
    ASSERT_THAT(raw.nullColumns, Eq(correct.nullColumns));
    ASSERT_THAT(raw.fixedLengths, Eq(correct.fixedLengths));
    ASSERT_THAT(raw.varLengths, Eq(correct.varLengths));
}

