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

TEST_F(TableMgrTest, duplicatedDbName)
{
    mgr.createDb("db");
    ASSERT_THROW(mgr.createDb("db"), IDAlreadyUsedException);
}

TEST_F(TableMgrTest, useNonExistingDb)
{
    ASSERT_THROW(mgr.use("db"), NoSuchThingException);
}

TEST_F(TableMgrTest, manageTables)
{
    mgr.createDb("db");
    mgr.use("db");
    mgr.createTable("table1", {});
    mgr.createTable("table2", {});
    mgr.createTable("table3", {});
    mgr.dropTable("table2");
    auto tables = mgr.showTables();
    ASSERT_THAT(tables.size(), Eq(2));
    ASSERT_THAT(tables[0]["table"]->toString(), Eq("table1"));
    ASSERT_THAT(tables[1]["table"]->toString(), Eq("table3"));
}

TEST_F(TableMgrTest, noDbInUse)
{
    ASSERT_THROW(mgr.createTable("table1", {}), NoDBInUseException);
}

