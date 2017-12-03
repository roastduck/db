#include <sstream>
#include "io/Input.h"
#include "io/Output.h"
#include "TableMgr.h"
#include "antlr4-runtime.h"

#include "filesystem/MockPageMgr.h"

#include "gmock/gmock.h"

using namespace testing;

class ParserTest : public Test
{
public:
    NiceMock<MockPageMgr> pageMgr;
    PageCache cache;
    TableMgr mgr;
    std::ostringstream outStream, errStream;
    Output output;
    Input input;

    ParserTest()
        : cache(pageMgr),
          mgr(cache),
          output(outStream, errStream),
          input(mgr, output)
    {}
};

TEST_F(ParserTest, syntaxError)
{
    input.parse("error syntex");
    ASSERT_THAT(errStream.str(), Eq("Syntax error\n"));
}

TEST_F(ParserTest, keywordCannotBeIdentifier)
{
    input.parse("CREATE DATABASE DATABASE;");
    ASSERT_THAT(errStream.str(), Eq("Syntax error\n"));
}

TEST_F(ParserTest, oneLetterIdentifier)
{
    // We are testing this because there are 1-letter lexical tokens
    input.parse("CREATE DATABASE a;");
    auto dbs = mgr.showDbs();
    ASSERT_THAT(dbs.size(), Eq(1));
    ASSERT_THAT(dbs[0][TableMgr::DB]->toString(), Eq("a"));
}

/************************************/
/* DB Managements                   */
/************************************/

TEST_F(ParserTest, createDb)
{
    input.parse("CREATE DATABASE db;");
    auto dbs = mgr.showDbs();
    ASSERT_THAT(dbs.size(), Eq(1));
    ASSERT_THAT(dbs[0][TableMgr::DB]->toString(), Eq("db"));
    ASSERT_THAT(outStream.str(), Eq("Created database db\n"));
}

TEST_F(ParserTest, lowerCase)
{
    input.parse("CrEaTe DaTAbASe db;");
    ASSERT_THAT(outStream.str(), Eq("Created database db\n"));
}

TEST_F(ParserTest, showDbs)
{
    input.parse("CREATE DATABASE db;");
    outStream.str("");
    input.parse("SHOW DATABASES;");
    ASSERT_THAT(outStream.str(), Eq(
        "+----------+\n"
        "| Database |\n"
        "+----------+\n"
        "| db       |\n"
        "+----------+\n"
    ));
}

TEST_F(ParserTest, dropDb)
{
    input.parse("CREATE DATABASE db;");
    outStream.str("");
    input.parse("DROP DATABASE db;");
    ASSERT_THAT(outStream.str(), Eq("Dropped database db\n"));
    outStream.str("");
    input.parse("SHOW DATABASES;");
    ASSERT_THAT(outStream.str(), Eq("(Empty set)\n"));
}

TEST_F(ParserTest, use)
{
    input.parse("CREATE DATABASE db;");
    outStream.str("");
    input.parse("USE db;");
    ASSERT_THAT(outStream.str(), Eq("Database changed to db\n"));
}

/************************************/
/* Table Managements                */
/************************************/

TEST_F(ParserTest, showTables)
{
    input.parse("CREATE DATABASE db; USE db; CREATE TABLE tb();");
    outStream.str("");
    input.parse("SHOW TABLES;");
    ASSERT_THAT(outStream.str(), Eq(
        "+-------+\n"
        "| Table |\n"
        "+-------+\n"
        "| tb    |\n"
        "+-------+\n"
    ));
}

TEST_F(ParserTest, multiplePrimary)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT, b INT, PRIMARY KEY (a), PRIMARY KEY (b));");
    ASSERT_THAT(errStream.str(), Eq("There can only be at most one primary index in a table\n"));
}

TEST_F(ParserTest, dropTable)
{
    input.parse("CREATE DATABASE db; USE db; CREATE TABLE tb();");
    outStream.str("");
    input.parse("DROP TABLE tb;");
    ASSERT_THAT(outStream.str(), Eq("Dropped table tb\n"));
    outStream.str("");
    input.parse("SHOW TABLES;");
    outStream.str("(Empty set)");
}

TEST_F(ParserTest, desc)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT NOT NULL, b VARCHAR(5), c DATE, PRIMARY KEY (a,b));");
    outStream.str("");
    input.parse("DESC tb;");
    ASSERT_THAT(outStream.str(), Eq(
        "+-------+------------+----------+-------------+\n"
        "| Field | Type       | Not null | Primary key |\n"
        "+-------+------------+----------+-------------+\n"
        "| a     | INT        | YES      | YES         |\n"
        "+-------+------------+----------+-------------+\n"
        "| b     | VARCHAR(5) | NO       | YES         |\n"
        "+-------+------------+----------+-------------+\n"
        "| c     | DATE       | NO       | NO          |\n"
        "+-------+------------+----------+-------------+\n"
    ));
}

/************************************/
/* Queries                          */
/************************************/

TEST_F(ParserTest, insert)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT, b VARCHAR(5));");
    outStream.str("");
    input.parse("INSERT INTO tb VALUES (1, NULL), (2, 'X');");
    ASSERT_THAT(outStream.str(), Eq("Inserted 2 record(s) into table tb\n"));
    auto result = mgr.select(
        { std::make_pair("tb", Table::Index({"a", "b"})) },
        { "tb" },
        {}
    );
    ASSERT_THAT(result.size(), Eq(2));
    ASSERT_THAT(result[0]["tb.a"]->toString(), Eq("1"));
    ASSERT_THAT(result[0]["tb.b"], Eq(nullptr));
    ASSERT_THAT(result[1]["tb.a"]->toString(), Eq("2"));
    ASSERT_THAT(result[1]["tb.b"]->toString(), Eq("X"));
}

TEST_F(ParserTest, remove)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT);");
    input.parse("INSERT INTO tb VALUES (1), (2), (3), (4), (NULL);");
    outStream.str("");
    input.parse("DELETE FROM tb WHERE a >= 3;");
    ASSERT_THAT(outStream.str(), Eq("Deleted from table tb\n"));
    auto result = mgr.select(
        { std::make_pair("tb", Table::Index({"a"})) },
        { "tb" },
        {}
    );
    ASSERT_THAT(result.size(), Eq(3));
    ASSERT_THAT(result[0]["tb.a"]->toString(), Eq("1"));
    ASSERT_THAT(result[1]["tb.a"]->toString(), Eq("2"));
    ASSERT_THAT(result[2]["tb.a"], Eq(nullptr));
}

TEST_F(ParserTest, removeWithComparasionBetweenColumns)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT, b INT);");
    input.parse("INSERT INTO tb VALUES (0,1), (2,2), (3,3);");
    input.parse("DELETE FROM tb WHERE a=b AND a<3;");
    auto result = mgr.select(
        { std::make_pair("tb", Table::Index({"a"})) },
        { "tb" },
        {}
    );
    ASSERT_THAT(result.size(), Eq(2));
    ASSERT_THAT(result[0]["tb.a"]->toString(), Eq("0"));
    ASSERT_THAT(result[1]["tb.a"]->toString(), Eq("3"));
}

TEST_F(ParserTest, removeWithExplicitField)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT);");
    input.parse("INSERT INTO tb VALUES (1), (2);");
    input.parse("DELETE FROM tb WHERE tb.a=1;");
    auto result = mgr.select(
        { std::make_pair("tb", Table::Index({"a"})) },
        { "tb" },
        {}
    );
    ASSERT_THAT(result.size(), Eq(1));
    ASSERT_THAT(result[0]["tb.a"]->toString(), Eq("2"));
}

TEST_F(ParserTest, removeWithWrongTableDotField)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT);");
    input.parse("INSERT INTO tb VALUES (1), (2);");
    input.parse("DELETE FROM tb WHERE wrong.a=1;");
    ASSERT_THAT(errStream.str(), "Illegal field: wrong.a\n");
    auto result = mgr.select(
        { std::make_pair("tb", Table::Index({"a"})) },
        { "tb" },
        {}
    );
    ASSERT_THAT(result.size(), Eq(2));
}

TEST_F(ParserTest, removeIsNull)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT);");
    input.parse("INSERT INTO tb VALUES (1), (NULL);");
    input.parse("DELETE FROM tb WHERE a IS NULL;");
    auto result = mgr.select(
        { std::make_pair("tb", Table::Index({"a"})) },
        { "tb" },
        {}
    );
    ASSERT_THAT(result.size(), Eq(1));
    ASSERT_THAT(result[0]["tb.a"]->toString(), Eq("1"));
}

TEST_F(ParserTest, removeEqualsNull)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT);");
    input.parse("INSERT INTO tb VALUES (1), (NULL);");
    input.parse("DELETE FROM tb WHERE a = NULL;");
    auto result = mgr.select(
        { std::make_pair("tb", Table::Index({"a"})) },
        { "tb" },
        {}
    );
    ASSERT_THAT(result.size(), Eq(2));
    ASSERT_THAT(result[0]["tb.a"]->toString(), Eq("1"));
    ASSERT_THAT(result[1]["tb.a"], Eq(nullptr));
}

TEST_F(ParserTest, update)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(PRIMARY KEY(b), a INT, b INT);");
    input.parse("INSERT INTO tb VALUES (0,1), (2,2), (3,3), (4,4);");
    outStream.str("");
    input.parse("UPDATE tb SET a = 5 WHERE a < 4 AND a = b;");
    ASSERT_THAT(outStream.str(), Eq("Updated table tb\n"));
    auto result = mgr.select(
        { std::make_pair("tb", Table::Index({"a", "b"})) },
        { "tb" },
        {}
    );
    ASSERT_THAT(result.size(), Eq(4));
    ASSERT_THAT(result[0]["tb.a"]->toString(), Eq("0"));
    ASSERT_THAT(result[0]["tb.b"]->toString(), Eq("1"));
    ASSERT_THAT(result[1]["tb.a"]->toString(), Eq("5"));
    ASSERT_THAT(result[1]["tb.b"]->toString(), Eq("2"));
    ASSERT_THAT(result[2]["tb.a"]->toString(), Eq("5"));
    ASSERT_THAT(result[2]["tb.b"]->toString(), Eq("3"));
    ASSERT_THAT(result[3]["tb.a"]->toString(), Eq("4"));
    ASSERT_THAT(result[3]["tb.b"]->toString(), Eq("4"));
}

