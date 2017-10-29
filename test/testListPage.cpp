#include "Type.h"
#include "Column.h"
#include "ListPage.h"
#include "type/CharType.h"
#include "exception/NotNullException.h"
#include "exception/NotInDomainException.h"

#include "filesystem/MockPageMgr.h"

#include "gmock/gmock.h"

using namespace testing;

class ListPageTest : public Test
{
public:
    MockPageMgr pageMgr;
    PageCache cache;
    ListPage page;

    ListPageTest()
        : cache(pageMgr),
          page(cache, "filename", 0, {
                (Column){ Type::INT, 0, "int", false, {} }, // fixed 4
                (Column){ Type::CHAR, 10, "varchar1", false, {} },
                (Column){ Type::CHAR, 2, "xxORyy", true, { "xx", "yy" } }, // fixed 2, not null
                (Column){ Type::CHAR, 10, "varchar2", false, {} } // now we are treating VARCHAR as CHAR
          })
        {}
};

TEST_F(ListPageTest, setAndGet0)
{
    page.setValue(0, "int", Type::newFromBytes({0x01, 0x02, 0x03, 0x04}, Type::INT));
    page.setValue(0, "varchar1", nullptr);
    page.setValue(0, "xxORyy", Type::newFromBytes({'x', 'x'}, Type::CHAR, 2));
    page.setValue(0, "varchar2", Type::newFromLiteral({0x34, 0x35, 0x36}, Type::CHAR, 10));

    ASSERT_TRUE(*page.getValue(0, "int") == *Type::newFromBytes({0x01, 0x02, 0x03, 0x04}, Type::INT));
    ASSERT_TRUE(page.getValue(0, "varchar1") == nullptr);
    ASSERT_TRUE(*page.getValue(0, "xxORyy") == *Type::newFromBytes({'x', 'x'}, Type::CHAR, 2));
    ASSERT_TRUE(*page.getValue(0, "varchar2") == *Type::newFromLiteral({0x34, 0x35, 0x36}, Type::CHAR, 10));
}

TEST_F(ListPageTest, move)
{
    page.setValue(0, "int", Type::newFromBytes({0x01, 0x02, 0x03, 0x04}, Type::INT));
    page.setValue(0, "varchar1", nullptr);
    page.setValue(0, "xxORyy", Type::newFromBytes({'x', 'x'}, Type::CHAR, 2));
    page.setValue(0, "varchar2", Type::newFromLiteral({0x34, 0x35, 0x36}, Type::CHAR, 10));

    page.copy(0, 1);

    ASSERT_TRUE(*page.getValue(1, "int") == *Type::newFromBytes({0x01, 0x02, 0x03, 0x04}, Type::INT));
    ASSERT_TRUE(page.getValue(1, "varchar1") == nullptr);
    ASSERT_TRUE(*page.getValue(1, "xxORyy") == *Type::newFromBytes({'x', 'x'}, Type::CHAR, 2));
    ASSERT_TRUE(*page.getValue(1, "varchar2") == *Type::newFromLiteral({0x34, 0x35, 0x36}, Type::CHAR, 10));
}

TEST_F(ListPageTest, notNullException)
{
    ASSERT_THROW(page.setValue(0, "xxORyy", nullptr), NotNullException);
}

TEST_F(ListPageTest, notInDomainException)
{
    ASSERT_THROW(page.setValue(0, "xxORyy", Type::newFromBytes({'z', 'z'}, Type::CHAR, 2)), NotInDomainException);
}

