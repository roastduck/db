#include <string>
#include "TableMgr.h"

#include "filesystem/MockPageMgr.h"

#include "gmock/gmock.h"

using namespace testing;

class TableMgrTest : public Test
{
public:
    MockPageMgr pageMgr;
    PageCache cache;
    TableMgr mgr;

    TableMgrTest()
        : cache(pageMgr),
          mgr(cache)
    {}
};

TEST_F(TableMgrTest, manageDbs)
{
    mgr.createDb("db1");
    mgr.createDb("db2");
    mgr.createDb("db3");
    mgr.dropDb("db2");
    auto dbs = mgr.showDbs();
    ASSERT_THAT(dbs.size(), Eq(2));
    ASSERT_THAT(dbs[0]["db"]->toString(), Eq("db1"));
    ASSERT_THAT(dbs[1]["db"]->toString(), Eq("db3"));
}

