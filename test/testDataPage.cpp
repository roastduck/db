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

