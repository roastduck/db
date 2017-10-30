#include <string>
#include <unordered_map>
#include "Table.h"

#include "filesystem/MockPageMgr.h"

#include "gmock/gmock.h"

using namespace testing;

class TableTest : public Test
{
public:
    typedef std::unordered_map<std::string, std::string> mapSS;
    typedef Table::ConsL mapSC;

    MockPageMgr pageMgr;
    PageCache cache;
    Table table;

    TableTest()
        : cache(pageMgr), table(cache, "table", {
                std::make_pair("int", (Column){ Type::INT, 0, false, {} }),
                std::make_pair("char", (Column){ Type::CHAR, 500, false, {} })
          })
        {}
};

TEST_F(TableTest, saveAndLoad)
{
    table.insert(mapSS({std::make_pair("int", "123"), std::make_pair("char", "123")}));
    auto result = table.select({"int", "char"}, mapSC({}));
    ASSERT_THAT(result.size(), Eq(1));
    ASSERT_TRUE(*result[0]["int"] == *Type::newFromLiteral("123", Type::INT));
    ASSERT_TRUE(*result[0]["char"] == *Type::newFromLiteral("123", Type::CHAR, 500));
}

TEST_F(TableTest, remove)
{
    table.insert(mapSS({std::make_pair("int", "123")}));
    table.insert(mapSS({std::make_pair("int", "456")}));
    table.remove(mapSC({std::make_pair("int", std::vector<Table::ConLiteral>({{Table::EQ, "123"}}))}));
    auto result = table.select({"int"}, mapSC({}));
    ASSERT_THAT(result.size(), Eq(1));
    ASSERT_TRUE(*result[0]["int"] == *Type::newFromLiteral("456", Type::INT));
}

TEST_F(TableTest, largerThan1Page)
{
    for (int i = 0; i < 100; i++)
        table.insert(mapSS({std::make_pair("int", std::to_string(i))}));
    table.remove(mapSC({std::make_pair("int", std::vector<Table::ConLiteral>({
        {Table::GE, "10"},
        {Table::LT, "60"}
    }))}));
    auto result = table.select({"int"}, mapSC({std::make_pair("int", std::vector<Table::ConLiteral>({
        {Table::GE, "50"},
        {Table::LT, "80"}
    }))}));
    ASSERT_THAT(result.size(), Eq(20));
}

