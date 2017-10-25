#include "DataPage.h"
#include "filesystem/PageCache.h"

#include "filesystem/MockPageMgr.h"

#include "gmock/gmock.h"

using namespace testing;

class DataPageTest : public Test
{
public:
    const int nullColumns = 10, fixedLengths = 8, varLengths = 2;
    MockPageMgr pageMgr;
    PageCache cache;
    DataPage page;

    DataPageTest()
        : cache(pageMgr), page(cache, "file", 0, nullColumns, fixedLengths, varLengths) {}
};

TEST_F(DataPageTest, embedAndPack)
{
    RawRecord raw;
    raw.nullColumns = {0, 1, 1, 0, 1, 1, 1, 0, 0, 1};
    raw.fixedLengths = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    raw.varLengths = {{'1', '2', '3', '4'}, {'X', 'Y', 'Z'}};
    page.addRecord(raw);
    auto _raw = page.readRecord(0);
    ASSERT_THAT(_raw.nullColumns, Eq(raw.nullColumns));
    ASSERT_THAT(_raw.fixedLengths, Eq(raw.fixedLengths));
    ASSERT_THAT(_raw.varLengths, Eq(raw.varLengths));
}

TEST_F(DataPageTest, remove)
{
    /*  t         1
     *  |     2   1
     *  |     2
     *  v     2 3
     *        2 3 4
     *      5 2 3 4
     */
    RawRecord raw1, raw2, raw3, raw4, raw5;
    raw1.nullColumns.resize(nullColumns, 0);
    raw1.fixedLengths.resize(fixedLengths, 1);
    raw1.varLengths = { std::vector<unsigned char>(30), {} };
    raw2.nullColumns.resize(nullColumns, 0);
    raw2.fixedLengths.resize(fixedLengths, 2);
    raw2.varLengths = { std::vector<unsigned char>(30), {} };
    raw3.nullColumns.resize(nullColumns, 0);
    raw3.fixedLengths.resize(fixedLengths, 3);
    raw3.varLengths = {{}, {}};
    raw4.nullColumns.resize(nullColumns, 0);
    raw4.fixedLengths.resize(fixedLengths, 4);
    raw4.varLengths = {{}, {}};
    raw5.nullColumns.resize(nullColumns, 0);
    raw5.fixedLengths.resize(fixedLengths, 5);
    raw5.varLengths = { std::vector<unsigned char>(30), {} };
    ASSERT_THAT(page.addRecord(raw1), Ne(-1));
    ASSERT_THAT(page.addRecord(raw2), Ne(-1));
    ASSERT_THAT(page.readRecord(0).fixedLengths, Eq(raw1.fixedLengths));
    page.delRecord(page.getRecPos(0));
    ASSERT_THAT(page.addRecord(raw3), Ne(-1));
    ASSERT_THAT(page.addRecord(raw4), Ne(-1));
    ASSERT_THAT(page.addRecord(raw5), Ne(-1));
    ASSERT_THAT(page.getSize(), Eq(4));
    ASSERT_THAT(page.readRecord(0).fixedLengths, Eq(raw4.fixedLengths));
    ASSERT_THAT(page.readRecord(1).fixedLengths, Eq(raw3.fixedLengths));
    ASSERT_THAT(page.readRecord(2).fixedLengths, Eq(raw2.fixedLengths));
    ASSERT_THAT(page.readRecord(3).fixedLengths, Eq(raw5.fixedLengths));
}

