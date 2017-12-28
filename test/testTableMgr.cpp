#include <string>
#include "TableMgr.h"

#include "filesystem/MockPageMgr.h"
#include "exception/RecordTooLargeException.h"

#include "gmock/gmock.h"

using namespace testing;

class TableMgrTest : public Test
{
public:
    NiceMock<MockPageMgr> pageMgr;
    PageCache cache;
    TableMgr mgr;

    TableMgrTest()
        : cache(pageMgr),
          mgr(cache)
    {}
};

/************************************/
/* DB Managements                   */
/************************************/

TEST_F(TableMgrTest, manageDbs)
{
    mgr.createDb("db1");
    mgr.createDb("db2");
    mgr.createDb("db3");
    mgr.dropDb("db2");
    auto dbs = mgr.showDbs();
    ASSERT_THAT(dbs.size(), Eq(2));
    ASSERT_THAT(dbs[0][TableMgr::DB]->toString(), Eq("db1"));
    ASSERT_THAT(dbs[1][TableMgr::DB]->toString(), Eq("db3"));
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

/************************************/
/* Table Managements                */
/************************************/

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
    ASSERT_THAT(tables[0][TableMgr::TABLE]->toString(), Eq("table1"));
    ASSERT_THAT(tables[1][TableMgr::TABLE]->toString(), Eq("table3"));
}

TEST_F(TableMgrTest, recordTooLarge)
{
    mgr.createDb("db");
    mgr.use("db");
    ASSERT_THROW(mgr.createTable(
        "table",
        { std::make_pair("c", (Column){ Type::CHAR, 5000, false }) }
    ), RecordTooLargeException);
    auto tables = mgr.showTables();
    ASSERT_THAT(tables.size(), Eq(0));
}

TEST_F(TableMgrTest, noDbInUse)
{
    ASSERT_THROW(mgr.createTable("table1", {}), NoDBInUseException);
}

/************************************/
/* Queries                          */
/************************************/

TEST_F(TableMgrTest, notNullExceptionDuringInsertion)
{
    mgr.createDb("db");
    mgr.use("db");
    mgr.createTable(
        "table",
        {
            std::make_pair("int1", (Column){ Type::INT, 0, false }),
            std::make_pair("int2", (Column){ Type::INT, 0, true })
        }
    );
    mgr.insert("table", {{ None(), Optional<std::string>("1") }}); // This is OK
    ASSERT_THROW(
        mgr.insert("table", {{ Optional<std::string>("1"), None() }}),
        NotNullException
    );
    auto result = mgr.select({}, { "table" }, {});
    ASSERT_THAT(result.size(), Eq(1)); // Broken row doesn't inserted
}

TEST_F(TableMgrTest, foreignKeyViolationDuringInsertion)
{
    mgr.createDb("db");
    mgr.use("db");
    mgr.createTable(
        "master",
        { std::make_pair("int", (Column){ Type::INT, 0, true }) },
        { Table::Index({"int"}) }
    );
    mgr.createTable(
        "slave",
        { std::make_pair("int", (Column){ Type::INT, 0, true }) },
        None(),
        {},
        { (TableMgr::ForeignKey){ "master", Table::Index({"int"}), Table::Index({"int"}) } }
    );
    ASSERT_THROW(
        mgr.insert("slave", {{ Optional<std::string>("1") }}),
        ForeignKeyViolatedException
    );
}

TEST_F(TableMgrTest, valueLengthNotMatch)
{
    mgr.createDb("db");
    mgr.use("db");
    mgr.createTable(
        "table",
        {
            std::make_pair("a", (Column){ Type::INT, 0, true }),
            std::make_pair("b", (Column){ Type::INT, 0, true }),
            std::make_pair("c", (Column){ Type::INT, 0, true })
        }
    );
    ASSERT_THROW(
        mgr.insert("table", {{ Optional<std::string>("1"), Optional<std::string>("2") }}),
        ValueListLengthNotMatchException
    );
}

TEST_F(TableMgrTest, foreignKeyViolationDuringDeletion)
{
    mgr.createDb("db");
    mgr.use("db");
    mgr.createTable(
        "master",
        { std::make_pair("int", (Column){ Type::INT, 0, true }) },
        { Table::Index({"int"}) }
    );
    mgr.createTable(
        "slave",
        { std::make_pair("int", (Column){ Type::INT, 0, true }) },
        None(),
        {},
        { (TableMgr::ForeignKey){ "master", Table::Index({"int"}), Table::Index({"int"}) } }
    );
    mgr.insert("master", {{ Optional<std::string>("1") }});
    mgr.insert("master", {{ Optional<std::string>("2") }});
    mgr.insert("slave", {{ Optional<std::string>("1") }});
    mgr.remove("master", { std::make_pair("int", std::vector<Table::ConLiteral>({
        {Table::EQ, "2"}
    })) }); // this is OK
    ASSERT_THROW(
        mgr.remove("master", { std::make_pair("int", std::vector<Table::ConLiteral>({
            {Table::EQ, "1"}
        })) }),
        ForeignKeyViolatedException
    );
}

TEST_F(TableMgrTest, remove)
{
    mgr.createDb("db");
    mgr.use("db");
    mgr.createTable(
        "table",
        { std::make_pair("int", (Column){ Type::INT, 0, true }) }
    );
    mgr.insert("table", {{ Optional<std::string>("1") }});
    mgr.insert("table", {{ Optional<std::string>("2") }});
    mgr.remove("table", { std::make_pair("int", std::vector<Table::ConLiteral>({
        {Table::EQ, "1"}
    })) });
    auto result = mgr.select(
        { std::make_pair("table", Table::Index({"int"})) },
        { "table" },
        {}
    );
    ASSERT_THAT(result.size(), Eq(1));
    ASSERT_THAT(result[0]["table.int"]->toString(), Eq("2"));
}

TEST_F(TableMgrTest, notNullExceptionDuringUpdating)
{
    mgr.createDb("db");
    mgr.use("db");
    mgr.createTable(
        "table",
        {
            std::make_pair("int1", (Column){ Type::INT, 0, false }),
            std::make_pair("int2", (Column){ Type::INT, 0, true })
        }
    );
    mgr.insert("table", {{ Optional<std::string>("1"), Optional<std::string>("1") }});
    mgr.update("table", { std::make_pair("int1", None()) }, {}); // This is OK
    ASSERT_THROW(
        mgr.update("table", { std::make_pair("int2", None()) }, {}),
        NotNullException
    );
    auto result = mgr.select(
        { std::make_pair("table", Table::Index({"int2"})) },
        { "table" },
        {}
    );
    ASSERT_THAT(result.size(), Eq(1));
    ASSERT_THAT(result[0]["table.int2"], Ne(nullptr));
    ASSERT_THAT(result[0]["table.int2"]->toString(), Eq("1")); // Broken update not proformed
}

TEST_F(TableMgrTest, foreignKeyViolationDuringUpdateAsReferrer)
{
    mgr.createDb("db");
    mgr.use("db");
    mgr.createTable(
        "master",
        { std::make_pair("int", (Column){ Type::INT, 0, true }) },
        { Table::Index({"int"}) }
    );
    mgr.createTable(
        "slave",
        { std::make_pair("int", (Column){ Type::INT, 0, true }) },
        None(),
        {},
        { (TableMgr::ForeignKey){ "master", Table::Index({"int"}), Table::Index({"int"}) } }
    );
    mgr.insert("master", {{ Optional<std::string>("1") }});
        mgr.insert("slave", {{ Optional<std::string>("1") }});
    ASSERT_THROW(
        mgr.update(
            "slave",
            { std::make_pair("int", Optional<std::string>("4")) },
            { std::make_pair("int", std::vector<Table::ConLiteral>({{Table::EQ, "1"}})) }
        ),
        ForeignKeyViolatedException
    );
}

TEST_F(TableMgrTest, foreignKeyViolationDuringUpdateAsReferee)
{
    mgr.createDb("db");
    mgr.use("db");
    mgr.createTable(
        "master",
        { std::make_pair("int", (Column){ Type::INT, 0, true }) },
        { Table::Index({"int"}) }
    );
    mgr.createTable(
        "slave",
        { std::make_pair("int", (Column){ Type::INT, 0, true }) },
        None(),
        {},
        { (TableMgr::ForeignKey){ "master", Table::Index({"int"}), Table::Index({"int"}) } }
    );
    mgr.insert("master", {{ Optional<std::string>("1") }});
    mgr.insert("master", {{ Optional<std::string>("2") }});
    mgr.insert("slave", {{ Optional<std::string>("1") }});
    mgr.update(
        "master",
        { std::make_pair("int", Optional<std::string>("3")) },
        { std::make_pair("int", std::vector<Table::ConLiteral>({{Table::EQ, "2"}})) }
    ); // this is OK
    ASSERT_THROW(
        mgr.update(
            "master",
            { std::make_pair("int", Optional<std::string>("4")) },
            { std::make_pair("int", std::vector<Table::ConLiteral>({{Table::EQ, "1"}})) }
        ),
        ForeignKeyViolatedException
    );
}

TEST_F(TableMgrTest, update)
{
    mgr.createDb("db");
    mgr.use("db");
    mgr.createTable(
        "table",
        { std::make_pair("int", (Column){ Type::INT, 0, true }) },
        Table::Index({ "int" })
    );
    mgr.insert("table", {{ Optional<std::string>("1") }});
    mgr.insert("table", {{ Optional<std::string>("2") }});
    mgr.update(
        "table",
        { std::make_pair("int", Optional<std::string>("3")) },
        { std::make_pair("int", std::vector<Table::ConLiteral>({{Table::EQ, "1"}})) }
    );
    auto result = mgr.select(
        { std::make_pair("table", Table::Index({"int"})) },
        { "table" },
        {}
    );
    ASSERT_THAT(result.size(), Eq(2));
    ASSERT_THAT(result[0]["table.int"]->toString(), Eq("2"));
    ASSERT_THAT(result[1]["table.int"]->toString(), Eq("3"));
}

TEST_F(TableMgrTest, insertAndSelect)
{
    mgr.createDb("db");
    mgr.use("db");
    mgr.createTable(
        "table",
        { std::make_pair("int", (Column){ Type::INT, 0, true }) }
    );
    mgr.insert("table", {{ Optional<std::string>("1") }});
    mgr.insert("table", {{ Optional<std::string>("2") }});
    std::unordered_map< std::string, Table::ConsL > innerCons;
    innerCons["table"]["int"] = std::vector<Table::ConLiteral>({{Table::EQ, "1"}});
    auto result = mgr.select(
        { std::make_pair("table", Table::Index({"int"})) },
        { "table" },
        innerCons
    );
    ASSERT_THAT(result.size(), Eq(1));
    ASSERT_THAT(result[0]["table.int"]->toString(), Eq("1"));
}

TEST_F(TableMgrTest, insertOrder)
{
    mgr.createDb("db");
    mgr.use("db");
    mgr.createTable(
        "table",
        {
            std::make_pair("f", (Column){ Type::INT, 0, true }),
            std::make_pair("o", (Column){ Type::INT, 0, true }),
            std::make_pair("b", (Column){ Type::INT, 0, true }),
            std::make_pair("a", (Column){ Type::INT, 0, true }),
            std::make_pair("r", (Column){ Type::INT, 0, true }),
        }
    );
    mgr.insert("table", {{
        Optional<std::string>("1"),
        Optional<std::string>("2"),
        Optional<std::string>("3"),
        Optional<std::string>("4"),
        Optional<std::string>("5")
    }});
    auto result = mgr.select(
        { std::make_pair("table", Table::Index({"f", "o", "b", "a", "r"})) },
        { "table" },
        {}
    );
    ASSERT_THAT(result.size(), Eq(1));
    ASSERT_THAT(result[0]["table.f"]->toString(), Eq("1"));
    ASSERT_THAT(result[0]["table.o"]->toString(), Eq("2"));
    ASSERT_THAT(result[0]["table.b"]->toString(), Eq("3"));
    ASSERT_THAT(result[0]["table.a"]->toString(), Eq("4"));
    ASSERT_THAT(result[0]["table.r"]->toString(), Eq("5"));
}


TEST_F(TableMgrTest, twoTablesConnection)
{
    mgr.createDb("db");
    mgr.use("db");
    mgr.createTable(
        "t1",
        {
            std::make_pair("a", (Column){ Type::CHAR, 1, true }),
            std::make_pair("b", (Column){ Type::INT, 0, true })
        }
    );
    mgr.createTable(
        "t2",
        {
            std::make_pair("c", (Column){ Type::INT, 0, true }),
            std::make_pair("d", (Column){ Type::CHAR, 1, true })
        }
    );

    mgr.insert("t1", {{ Optional<std::string>("a"), Optional<std::string>("1") }});
    mgr.insert("t1", {{ Optional<std::string>("b"), Optional<std::string>("3") }});
    mgr.insert("t1", {{ Optional<std::string>("c"), Optional<std::string>("4") }});

    mgr.insert("t2", {{ Optional<std::string>("1"), Optional<std::string>("X") }});
    mgr.insert("t2", {{ Optional<std::string>("4"), Optional<std::string>("Y") }});
    mgr.insert("t2", {{ Optional<std::string>("5"), Optional<std::string>("Z") }});

    Table::OuterCon out;
    out.dir = Table::EQ;
    out.col1 = "b";
    out.col2 = "c";
    TableMgr::OuterConsMap outMap;
    outMap[std::pair<std::string, std::string>("t1", "t2")] = { out };
    auto result = mgr.select(
        {
            std::make_pair("t1", Table::Index({"a"})),
            std::make_pair("t2", Table::Index({"d"}))
        },
        { "t1", "t2" },
        {},
        outMap
    );

    ASSERT_THAT(result.size(), Eq(2));
    ASSERT_THAT(result[0]["t1.a"]->toString(), Eq("a"));
    ASSERT_THAT(result[0]["t2.d"]->toString(), Eq("X"));
    ASSERT_THAT(result[1]["t1.a"]->toString(), Eq("c"));
    ASSERT_THAT(result[1]["t2.d"]->toString(), Eq("Y"));
}

