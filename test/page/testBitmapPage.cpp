#include "page/BitmapPage.h"

#include "../filesystem/MockPageMgr.h"

#include "gmock/gmock.h"

using namespace testing;

class BitmapPageTest : public Test
{
public:
    NiceMock<MockPageMgr> pageMgr;
    PageCache cache;
    BitmapPage page;

    BitmapPageTest()
        : cache(pageMgr), page(cache, "filename", 0) {}
};

TEST_F(BitmapPageTest, setAndGet)
{
    ASSERT_TRUE(!page.get(35));
    page.set(35);
    ASSERT_TRUE(page.get(35));
}

TEST_F(BitmapPageTest, first0)
{
    for (int i = 0; i < 100; i++)
        page.set(i);
    page.reset(25);
    page.reset(46);
    page.reset(14);
    ASSERT_THAT(page.first0(), Eq(14));
}

