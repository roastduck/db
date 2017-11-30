#include "io/Input.h"
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
    Input input;

    ParserTest()
        : cache(pageMgr),
          mgr(cache),
          input(mgr)
    {}
};

TEST_F(ParserTest, syntaxError)
{
    ASSERT_THROW(input.parse("error syntex"), antlr4::ParseCancellationException);
}

TEST_F(ParserTest, keywordCannotBeIdentifier)
{
    ASSERT_THROW(input.parse("CREATE DATABASE DATABASE;"), antlr4::ParseCancellationException);
}

TEST_F(ParserTest, oneLetterIdentifier)
{
    // We are testing this because there are 1-letter lexical tokens
    input.parse("CREATE DATABASE a;");
    auto dbs = mgr.showDbs();
    ASSERT_THAT(dbs.size(), Eq(1));
    ASSERT_THAT(dbs[0]["db"]->toString(), Eq("a"));
}

TEST_F(ParserTest, createDb)
{
    input.parse("CREATE DATABASE db;");
    auto dbs = mgr.showDbs();
    ASSERT_THAT(dbs.size(), Eq(1));
    ASSERT_THAT(dbs[0]["db"]->toString(), Eq("db"));
}

