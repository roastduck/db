#include <sstream>
#include <algorithm>
#include "utils.h"
#include "config.h"
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
          output(outStream, outStream, errStream),
          input(mgr, output)
    {}
};

TEST_F(ParserTest, syntaxError)
{
    input.parse("error syntex");
    ASSERT_THAT(errStream.str(), Eq("Syntax error\n"));
}

TEST_F(ParserTest, typeError)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INTEGER);");
    ASSERT_THAT(errStream.str(), Eq("No such type named INTEGER\n"));
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

TEST_F(ParserTest, stringWithEscaping)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(s CHAR(20));");
    input.parse("INSERT INTO tb VALUES('Mary\\'s backslash \\'\\\\\\'');");
    outStream.str("");
    input.parse("SELECT * FROM tb WHERE s IS NOT NULL;");
    ASSERT_THAT(outStream.str(), Eq(
        "+----------------------+\n"
        "| tb.s                 |\n"
        "+----------------------+\n"
        "| Mary's backslash '\\' |\n"
        "+----------------------+\n"
    ));
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

TEST_F(ParserTest, typeNameAsFieldName)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(date DATE);");
    input.parse("INSERT INTO tb VALUES('2018-1-2');");
    outStream.str("");
    input.parse("SELECT * FROM tb WHERE date IS NOT NULL;");
    ASSERT_THAT(outStream.str(), Eq(
        "+------------+\n"
        "| tb.date    |\n"
        "+------------+\n"
        "| 2018-01-02 |\n"
        "+------------+\n"
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
    ASSERT_THAT(errStream.str(), Eq(""));
    outStream.str("");
    input.parse("SHOW TABLES;");
    ASSERT_THAT(outStream.str(), Eq("(Empty set)\n"));
}

TEST_F(ParserTest, dropTableAsReferee)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE master(a INT NOT NULL, PRIMARY KEY(a));");
    input.parse("CREATE TABLE slave(b INT, FOREIGN KEY (b) REFERENCES master(a));");
    input.parse("INSERT INTO master VALUES (1);");
    input.parse("INSERT INTO slave VALUES (1);");

    errStream.str("");
    input.parse("DROP TABLE master;");
    ASSERT_THAT(errStream.str(), Eq("Foreign key constraint below is violated: slave (b) => master (a)\n"));

    input.parse("DELETE FROM slave WHERE b = 1;");
    errStream.str("");
    outStream.str("");
    input.parse("DROP TABLE master;");
    ASSERT_THAT(errStream.str(), Eq(""));
    ASSERT_THAT(outStream.str(), Eq("Dropped table master\n"));
}

TEST_F(ParserTest, dropShouldDeleteData)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT);");
    input.parse("INSERT INTO tb VALUES (1);");
    input.parse("DROP TABLE tb;");
    input.parse("CREATE TABLE tb(a INT);");
    outStream.str("");
    input.parse("SELECT * FROM tb WHERE a IS NOT NULL;");
    ASSERT_THAT(outStream.str(), Eq("(Empty set)\n"));
}

TEST_F(ParserTest, desc)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT NOT NULL, b VARCHAR(5) NOT NULL, c DATE, PRIMARY KEY (a,b));");
    outStream.str("");
    input.parse("DESC tb;");
    ASSERT_THAT(outStream.str(), Eq(
        "+-------+------------+----------+-------------+\n"
        "| Field | Type       | Not null | Primary key |\n"
        "+-------+------------+----------+-------------+\n"
        "| a     | INT        | YES      | YES         |\n"
        "+-------+------------+----------+-------------+\n"
        "| b     | VARCHAR(5) | YES      | YES         |\n"
        "+-------+------------+----------+-------------+\n"
        "| c     | DATE       | NO       | NO          |\n"
        "+-------+------------+----------+-------------+\n"
    ));
}

TEST_F(ParserTest, primaryNull)
{
    input.parse("CREATE DATABASE db; USE db;");
    errStream.str("");
    input.parse("CREATE TABLE tb(a INT, PRIMARY KEY (a));");
    ASSERT_THAT(errStream.str(), Eq("Column \"a\" cannot be NULL\n"));
}

TEST_F(ParserTest, primaryNotExists)
{
    input.parse("CREATE DATABASE db; USE db;");
    errStream.str("");
    input.parse("CREATE TABLE tb(a INT, PRIMARY KEY (b));");
    ASSERT_THAT(errStream.str(), Eq("No such field named b\n"));
}

TEST_F(ParserTest, foreign)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE master (a INT NOT NULL, PRIMARY KEY (a));");
    input.parse("CREATE TABLE slave (b INT NOT NULL, FOREIGN KEY (b) REFERENCES master(a));");
    errStream.str("");
    input.parse("INSERT INTO slave VALUES (1);");
    ASSERT_THAT(errStream.str(), Eq("Foreign key constraint below is violated: slave (b) => master (a)\n"));
}

TEST_F(ParserTest, duplicateIndex)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb (a INT, b INT);");
    input.parse("CREATE INDEX tb(a, b);");
    input.parse("CREATE INDEX tb(b, a);"); // This is OK
    input.parse("CREATE INDEX tb(a, b);");
    ASSERT_THAT(errStream.str(), Eq("Index tb(a,b) already exists\n"));
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
    input.parse("CREATE TABLE tb(PRIMARY KEY(b), a INT, b INT NOT NULL);");
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

TEST_F(ParserTest, selectAllFrom1Table)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(PRIMARY KEY(a,b,c), a INT NOT NULL, b INT NOT NULL, c INT NOT NULL);");
    input.parse("INSERT INTO tb VALUES (0,1,2), (2,2,1), (4,3,5);");
    outStream.str("");
    input.parse("SELECT * FROM tb WHERE a > 0;");
    ASSERT_THAT(outStream.str(), Eq(
        "+------+------+------+\n"
        "| tb.a | tb.b | tb.c |\n"
        "+------+------+------+\n"
        "| 2    | 2    | 1    |\n"
        "+------+------+------+\n"
        "| 4    | 3    | 5    |\n"
        "+------+------+------+\n"
    ));
}

TEST_F(ParserTest, selectSomeFrom1Table)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(PRIMARY KEY(a,b,c), a INT NOT NULL, b INT NOT NULL, c INT NOT NULL);");
    input.parse("INSERT INTO tb VALUES (0,1,2), (2,2,1), (4,3,5);");
    outStream.str("");
    input.parse("SELECT c, a FROM tb WHERE a > 0;");
    ASSERT_THAT(outStream.str(), Eq(
        "+------+------+\n"
        "| tb.c | tb.a |\n"
        "+------+------+\n"
        "| 1    | 2    |\n"
        "+------+------+\n"
        "| 5    | 4    |\n"
        "+------+------+\n"
    ));
}

TEST_F(ParserTest, selectSomeFrom1TableWithExplicitTableName)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(PRIMARY KEY(a,b,c), a INT NOT NULL, b INT NOT NULL, c INT NOT NULL);");
    input.parse("INSERT INTO tb VALUES (0,1,2), (2,2,1), (4,3,5);");
    outStream.str("");
    input.parse("SELECT tb.c, tb.a FROM tb WHERE tb.a > 0;");
    ASSERT_THAT(outStream.str(), Eq(
        "+------+------+\n"
        "| tb.c | tb.a |\n"
        "+------+------+\n"
        "| 1    | 2    |\n"
        "+------+------+\n"
        "| 5    | 4    |\n"
        "+------+------+\n"
    ));
}

TEST_F(ParserTest, selectAllFromMultipleTables)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE t1(PRIMARY KEY(a,b), a INT NOT NULL, b INT NOT NULL);");
    input.parse("INSERT INTO t1 VALUES (0,1), (2,2), (7,8);");
    input.parse("CREATE TABLE t2(PRIMARY KEY(x), x INT NOT NULL);");
    input.parse("INSERT INTO t2 VALUES (5), (4), (6);");
    outStream.str("");
    input.parse("SELECT * FROM t1, t2 WHERE t1.a < 5 AND t2.x >= 5;");
    ASSERT_THAT(outStream.str(), Eq(
        "+------+------+------+\n"
        "| t1.a | t1.b | t2.x |\n"
        "+------+------+------+\n"
        "| 0    | 1    | 5    |\n"
        "+------+------+------+\n"
        "| 0    | 1    | 6    |\n"
        "+------+------+------+\n"
        "| 2    | 2    | 5    |\n"
        "+------+------+------+\n"
        "| 2    | 2    | 6    |\n"
        "+------+------+------+\n"
    ));
}

TEST_F(ParserTest, selectSomeFromMultipleTables)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE t1(PRIMARY KEY(a,b), a INT NOT NULL, b INT NOT NULL);");
    input.parse("INSERT INTO t1 VALUES (0,1), (2,2), (7,8), (10,11);");
    input.parse("CREATE TABLE t2(PRIMARY KEY(x), x INT NOT NULL);");
    input.parse("INSERT INTO t2 VALUES (2), (7), (10);");
    outStream.str("");
    input.parse("SELECT t1.b FROM t1, t2 WHERE t1.a = t2.x AND t1.a < t1.b;");
    ASSERT_THAT(outStream.str(), Eq(
        "+------+\n"
        "| t1.b |\n"
        "+------+\n"
        "| 8    |\n"
        "+------+\n"
        "| 11   |\n"
        "+------+\n"
    ));
}

TEST_F(ParserTest, multiTableSelectionIllegalSelector)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE t1(PRIMARY KEY(a,b), a INT NOT NULL, b INT NOT NULL);");
    input.parse("CREATE TABLE t2(PRIMARY KEY(x), x INT NOT NULL);");
    input.parse("SELECT b FROM t1, t2 WHERE t1.a = t2.x;");
    ASSERT_THAT(errStream.str(), "Illegal field: b\n");
}

TEST_F(ParserTest, multiTableSelectionIllegalWhere)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE t1(PRIMARY KEY(a,b), a INT NOT NULL, b INT NOT NULL);");
    input.parse("CREATE TABLE t2(PRIMARY KEY(x), x INT NOT NULL);");
    input.parse("SELECT tb.b FROM t1, t2 WHERE t1.a = x;");
    ASSERT_THAT(errStream.str(), "Illegal field: x\n");
}

TEST_F(ParserTest, createAndDropIndex)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE t1(a FLOAT, b FLOAT, c FLOAT);");
    outStream.str("");
    input.parse("CREATE INDEX t1(a,b);");
    ASSERT_THAT(outStream.str(), "Created index (a,b) for table t1\n");
    outStream.str("");
    input.parse("DROP INDEX t1(a,b);");
    ASSERT_THAT(outStream.str(), "Deleted index (a,b) from table t1\n");
    input.parse("DROP INDEX t1(a,b);");
    ASSERT_THAT(errStream.str(), "No such index named (a,b)\n");
}

TEST_F(ParserTest, identifierTooLong)
{
    input.parse("CREATE DATABASE " + std::string(1000, 'x') + ";");
    ASSERT_THAT(errStream.str(), Eq(
        "Identifier `" + std::string(1000, 'x') + "` is too long. The maximum length allowed is " +
        std::to_string(MAX_IDENTIFIER_LEN) + " characters\n"
    ));
}

TEST_F(ParserTest, tooManyFields)
{
    input.parse("CREATE DATABASE db; USE db;");
    std::string s;
    for (int i = 0; i < 100; i++)
        s += (i ? ",c" : "c") + std::to_string(i) + " INT";
    input.parse("CREATE TABLE tb(" + s + ");");
    ASSERT_THAT(errStream.str(), Eq(
        "There are too many fields in the table (100 fields). The maximum is " + std::to_string(MAX_COLUMN_NUM) + "\n"
    ));
}

TEST_F(ParserTest, tooManyIndexes)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT, b INT, c INT, d INT, e INT, f INT);");
    std::vector<std::string> arr = {"a", "b", "c", "d", "e", "f"};
    for (int i = 0; i < MAX_INDEX_NUM + 1; i++)
    {
        input.parse("CREATE INDEX tb(" + commaJoin(arr) + ");");
        std::next_permutation(arr.begin(), arr.end());
    }
    ASSERT_THAT(errStream.str(), Eq(
        "There can't be more indexes than " + std::to_string(MAX_INDEX_NUM) + "\n"
    ));
}

TEST_F(ParserTest, wrongFieldWhenCreateIndex)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT);");
    input.parse("CREATE INDEX tb(b);");
    ASSERT_THAT(errStream.str(), Eq("No such field named b\n"));
}

TEST_F(ParserTest, wrongFieldWhenDelete)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT);");
    input.parse("INSERT INTO tb VALUES (1);");
    input.parse("DELETE FROM tb WHERE x = 1;");
    ASSERT_THAT(errStream.str(), Eq("No such field named x\n"));
    errStream.str("");
    input.parse("DELETE FROM tb WHERE a = y;");
    ASSERT_THAT(errStream.str(), Eq("No such field named y\n"));
    errStream.str("");
    input.parse("DELETE FROM tb WHERE z = a;");
    ASSERT_THAT(errStream.str(), Eq("No such field named z\n"));
    ASSERT_THAT(mgr.select({}, {"tb"}, {}).size(), Eq(1));
}

TEST_F(ParserTest, wrongFieldWhenUpdate)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT);");
    input.parse("INSERT INTO tb VALUES (1);");
    input.parse("UPDATE tb SET b = 1 WHERE a = 1;");
    ASSERT_THAT(errStream.str(), Eq("No such field named b\n"));
    errStream.str("");
    input.parse("UPDATE tb SET a = 1 WHERE x = 1;");
    ASSERT_THAT(errStream.str(), Eq("No such field named x\n"));
    errStream.str("");
    input.parse("UPDATE tb SET a = 1 WHERE a = y;");
    ASSERT_THAT(errStream.str(), Eq("No such field named y\n"));
    errStream.str("");
    input.parse("UPDATE tb SET a = 1 WHERE z = a;");
    ASSERT_THAT(errStream.str(), Eq("No such field named z\n"));
    ASSERT_THAT(mgr.select({}, {"tb"}, {}).size(), Eq(1));
}

TEST_F(ParserTest, wrongFieldWhenSelect)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT);");
    input.parse("INSERT INTO tb VALUES (1);");
    input.parse("SELECT b FROM tb WHERE a = 1;");
    ASSERT_THAT(errStream.str(), Eq("No such field named b\n"));
    errStream.str("");
    input.parse("SELECT * FROM tb WHERE x = 1;");
    ASSERT_THAT(errStream.str(), Eq("No such field named x\n"));
    errStream.str("");
    input.parse("SELECT * FROM tb WHERE a = y;");
    ASSERT_THAT(errStream.str(), Eq("No such field named y\n"));
    errStream.str("");
    input.parse("SELECT * FROM tb WHERE z = a;");
    ASSERT_THAT(errStream.str(), Eq("No such field named z\n"));
}

TEST_F(ParserTest, primaryNotUnique)
{
    // Init
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT NOT NULL, PRIMARY KEY (a));");
    input.parse("INSERT INTO tb VALUES (1), (2), (3);");

    // Error insertion
    input.parse("INSERT INTO tb VALUES (0), (2), (5);");
    ASSERT_THAT(errStream.str(), Eq("Column(s) (a) should be unique\n"));

    // Error update
    errStream.str("");
    input.parse("UPDATE tb SET a = 1 WHERE a = 3;");
    ASSERT_THAT(errStream.str(), Eq("Column(s) (a) should be unique\n"));
    errStream.str("");
    input.parse("UPDATE tb SET a = 4 WHERE a >= 2;");
    ASSERT_THAT(errStream.str(), Eq("Column(s) (a) should be unique\n"));

    // This update is OK
    errStream.str("");
    input.parse("UPDATE tb SET a = 1 WHERE a = 1;");
    ASSERT_THAT(errStream.str(), Eq(""));

    // Error commands should make no effect
    outStream.str("");
    input.parse("SELECT * FROM tb WHERE a IS NOT NULL;");
    ASSERT_THAT(outStream.str(), Eq(
        "+------+\n"
        "| tb.a |\n"
        "+------+\n"
        "| 1    |\n"
        "+------+\n"
        "| 2    |\n"
        "+------+\n"
        "| 3    |\n"
        "+------+\n"
    ));
}

TEST_F(ParserTest, checkConstraint)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT, b INT, CHECK (b IN (1, '2')));");

    input.parse("INSERT INTO tb VALUES (0, 1), ('-1', 2);"); // OK
    ASSERT_THAT(errStream.str(), Eq(""));

    input.parse("INSERT INTO tb VALUES (0, 3);");
    ASSERT_THAT(errStream.str(), Eq("CHECK constraint for tb(b) is violated\n"));

    errStream.str("");
    input.parse("UPDATE tb SET b = 4 WHERE a = 0;");
    ASSERT_THAT(errStream.str(), Eq("CHECK constraint for tb(b) is violated\n"));

    outStream.str("");
    input.parse("SELECT * FROM tb WHERE a IS NOT NULL;");
    ASSERT_THAT(outStream.str(), Eq(
        "+------+------+\n"
        "| tb.a | tb.b |\n"
        "+------+------+\n"
        "| 0    | 1    |\n"
        "+------+------+\n"
        "| -1   | 2    |\n"
        "+------+------+\n"
    ));
}

TEST_F(ParserTest, checkConstraintNull)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT, CHECK (a IN (1, NULL)));");
    ASSERT_THAT(errStream.str(), Eq("NULL in a CHECK constraint is currently not supported\n"));
    outStream.str("");
    input.parse("SHOW TABLES;");
    ASSERT_THAT(outStream.str(), Eq("(Empty set)\n"));
}

TEST_F(ParserTest, checkConstraintTooLong)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a CHAR(100), CHECK (a IN ('a', '" + std::string('x', 100) + "')));");
    ASSERT_THAT(errStream.str(), Eq(
        "CHECK candidate longer than " + std::to_string(MAX_CHECK_LEN) +
        " characters is currently not supported\n")
    );
    outStream.str("");
    input.parse("SHOW TABLES;");
    ASSERT_THAT(outStream.str(), Eq("(Empty set)\n"));
}

TEST_F(ParserTest, orderBy)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT, b INT, c INT);");
    input.parse("INSERT INTO tb VALUES (3,2,1), (1,3,2), (1,5,3), (1,4,6), (2,1,7);");
    outStream.str("");
    input.parse("SELECT * FROM tb WHERE a IS NOT NULL ORDER BY a, b;");
    ASSERT_THAT(errStream.str(), Eq(""));
    ASSERT_THAT(outStream.str(), Eq(
        "+------+------+------+\n"
        "| tb.a | tb.b | tb.c |\n"
        "+------+------+------+\n"
        "| 1    | 3    | 2    |\n"
        "+------+------+------+\n"
        "| 1    | 4    | 6    |\n"
        "+------+------+------+\n"
        "| 1    | 5    | 3    |\n"
        "+------+------+------+\n"
        "| 2    | 1    | 7    |\n"
        "+------+------+------+\n"
        "| 3    | 2    | 1    |\n"
        "+------+------+------+\n"
    ));
}

TEST_F(ParserTest, orderByNonExistingField)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT);");
    input.parse("INSERT INTO tb VALUES (1), (2), (3);");
    input.parse("SELECT * FROM tb WHERE a IS NOT NULL ORDER BY b;");
    ASSERT_THAT(errStream.str(), Eq("No such field named tb.b\n"));
}

TEST_F(ParserTest, aggregate)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT, b INT);");
    input.parse("INSERT INTO tb VALUES (1, 1), (2, 3), (3, 5);");

    outStream.str("");
    input.parse("SELECT SUM(a), AVG(b) FROM tb WHERE a IS NOT NULL;");
    ASSERT_THAT(outStream.str(), Eq(
        "+------+------+\n"
        "| tb.a | tb.b |\n"
        "+------+------+\n"
        "| 6    | 3    |\n"
        "+------+------+\n"
    ));

    outStream.str("");
    input.parse("SELECT MAX(a), MIN(b) FROM tb WHERE a IS NOT NULL;");
    ASSERT_THAT(outStream.str(), Eq(
        "+------+------+\n"
        "| tb.a | tb.b |\n"
        "+------+------+\n"
        "| 3    | 1    |\n"
        "+------+------+\n"
    ));
}

TEST_F(ParserTest, aggregateNull)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT, b INT, c INT);");
    input.parse("INSERT INTO tb VALUES (1, NULL, NULL), (NULL, 3, NULL), (2, 5, NULL);");

    outStream.str("");
    input.parse("SELECT SUM(a), AVG(b) FROM tb WHERE c IS NULL;");
    ASSERT_THAT(outStream.str(), Eq(
        "+------+------+\n"
        "| tb.a | tb.b |\n"
        "+------+------+\n"
        "| 3    | 4    |\n"
        "+------+------+\n"
    ));
}

TEST_F(ParserTest, aggregateMoreTypes)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(d DATE);");
    input.parse("INSERT INTO tb VALUES ('2017-1-5'), ('2017-12-17'), ('2017-3-9');");

    outStream.str("");
    input.parse("SELECT MAX(d) FROM tb WHERE d IS NOT NULL;");
    ASSERT_THAT(outStream.str(), Eq(
        "+------------+\n"
        "| tb.d       |\n"
        "+------------+\n"
        "| 2017-12-17 |\n"
        "+------------+\n"
    ));

    errStream.str("");
    input.parse("SELECT SUM(d) FROM tb WHERE d IS NOT NULL;");
    ASSERT_THAT(errStream.str(), Eq("Invalid type DATE for SUM or AVG\n"));
}

TEST_F(ParserTest, groupBy)
{
    input.parse("CREATE DATABASE db; USE db;");
    input.parse("CREATE TABLE tb(a INT, b INT, c INT);");
    input.parse("INSERT INTO tb VALUES (3,2,1), (1,3,1), (1,5,3), (1,4,7), (2,1,3);");
    outStream.str("");
    input.parse("SELECT SUM(a), AVG(b), c FROM tb WHERE c IS NOT NULL GROUP BY c ORDER BY c;");
    ASSERT_THAT(errStream.str(), Eq(""));
    ASSERT_THAT(outStream.str(), Eq(
        "+------+------+------+\n"
        "| tb.a | tb.b | tb.c |\n"
        "+------+------+------+\n"
        "| 4    | 2.5  | 1    |\n"
        "+------+------+------+\n"
        "| 3    | 3    | 3    |\n"
        "+------+------+------+\n"
        "| 1    | 4    | 7    |\n"
        "+------+------+------+\n"
    ));
}

