#include "Type.h"
#include "Column.h"
#include "page/ListPage.h"
#include "type/CharType.h"
#include "exception/NotNullException.h"

#include "../filesystem/MockPageMgr.h"

#include "gmock/gmock.h"

using namespace testing;

class ListPageTest : public Test
{
public:
    NiceMock<MockPageMgr> pageMgr;
    PageCache cache;
    ListPage page;

    ListPageTest()
        : cache(pageMgr),
          page(cache, "filename", 0, {
                std::make_pair("int", (Column){ Type::INT, 0, false }), // fixed 4
                std::make_pair("varchar1", (Column){ Type::CHAR, 10, false }),
                std::make_pair("xxORyy", (Column){ Type::CHAR, 2, true }), // fixed 2, not null
                std::make_pair("varchar2", (Column){ Type::CHAR, 10, false }) // now we are treating VARCHAR as CHAR
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

TEST_F(ListPageTest, setAndGetMetadata)
{
    page.setSize(1);
    page.setPrev(2);
    page.setNext(3);
    ASSERT_THAT(page.getSize(), Eq(1));
    ASSERT_THAT(page.getPrev(), Eq(2));
    ASSERT_THAT(page.getNext(), Eq(3));
}

