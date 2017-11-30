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
    ASSERT_THAT(errStream.str(), Eq("Syntax error"));
}

TEST_F(ParserTest, keywordCannotBeIdentifier)
{
    input.parse("CREATE DATABASE DATABASE;");
    ASSERT_THAT(errStream.str(), Eq("Syntax error"));
}

TEST_F(ParserTest, oneLetterIdentifier)
{
    // We are testing this because there are 1-letter lexical tokens
    input.parse("CREATE DATABASE a;");
    auto dbs = mgr.showDbs();
    ASSERT_THAT(dbs.size(), Eq(1));
    ASSERT_THAT(dbs[0][TableMgr::DB]->toString(), Eq("a"));
}

TEST_F(ParserTest, createDb)
{
    input.parse("CREATE DATABASE db;");
    auto dbs = mgr.showDbs();
    ASSERT_THAT(dbs.size(), Eq(1));
    ASSERT_THAT(dbs[0][TableMgr::DB]->toString(), Eq("db"));
}

