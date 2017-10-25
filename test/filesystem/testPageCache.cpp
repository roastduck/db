#include "filesystem/PageCache.h"

#include "MockPageMgr.h"

#include "gmock/gmock.h"

using namespace testing;

class PageCacheTest : public Test
{
public:
    MockPageMgr pageMgr;
    PageCache pageCache;

    PageCacheTest() : pageCache(pageMgr, 2) {}
};

TEST_F(PageCacheTest, dereference)
{
    EXPECT_CALL(pageMgr, read("a", 0, _)); // This line should be put before calling
    EXPECT_CALL(pageMgr, write("a", 0, _)); // During destruction
    auto iterW = pageCache.getPage("a", 0);
    auto iterR = pageCache.getConstPage("a", 0);
    iterW[0] = 1;
    *(iterW + 1) = 2;
    ASSERT_THAT(*iterW, Eq(1));
    ASSERT_THAT(*(iterW + 1), Eq(2));
    ASSERT_THAT(iterR[0], Eq(1));
    ASSERT_THAT(iterR[1], Eq(2));
}

TEST_F(PageCacheTest, replacement)
{
    {
        InSequence dummy;
        EXPECT_CALL(pageMgr, read("a", 2, _));
        EXPECT_CALL(pageMgr, read("a", 1, _));
        EXPECT_CALL(pageMgr, write("a", 2, _));
        EXPECT_CALL(pageMgr, read("a", 0, _));
        EXPECT_CALL(pageMgr, read("a", 2, _)); // 0 is const, so no need to write
        EXPECT_CALL(pageMgr, write("a", 2, _)); // during destruction
    }
    auto iterR0 = pageCache.getConstPage("a", 0);
    auto iterR1 = pageCache.getConstPage("a", 1);
    auto iterW2 = pageCache.getPage("a", 2);
    *iterW2;
    *iterR1;
    *iterR0;
    *iterW2;
}

TEST_F(PageCacheTest, relational)
{
    auto iterW = pageCache.getPage("a", 0);
    auto iterR = pageCache.getConstPage("a", 0);
    ASSERT_TRUE(iterW == iterR);
    ASSERT_TRUE(!(iterW != iterR));
    ASSERT_TRUE(iterW + 5 == iterR + 5);
    ASSERT_TRUE(!(iterW + 5 != iterR + 5));
    ASSERT_TRUE(iterW + 5 > iterR);
    ASSERT_TRUE(iterW < iterR + 5);
    ASSERT_THAT(5 + iterW - iterW, Eq(5));
    ASSERT_TRUE(iterW + 5 - 5 == iterW);
}

TEST_F(PageCacheTest, increment)
{
    auto iterR0 = pageCache.getConstPage("a", 0);
    auto iterR1 = iterR0;
    ASSERT_TRUE(iterR1++ == iterR0);
    ASSERT_TRUE(++iterR1 == iterR0 + 2);
    ASSERT_TRUE((iterR1 -= 2) == iterR0);
}

