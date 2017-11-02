#include <string>
#include <unordered_map>
#include "Table.h"

#include "filesystem/MockPageMgr.h"

#include "gmock/gmock.h"

using namespace testing;

class TableTest : public Test
{
public:
    MockPageMgr pageMgr;
    PageCache cache;
    Table table, tablePri;

    TableTest()
        : cache(pageMgr),
          table(cache, "table", {
                std::make_pair("int", (Column){ Type::INT, 0, false, {} }),
                std::make_pair("char", (Column){ Type::CHAR, 500, false, {} })
          }),
          tablePri(cache, "tablePri", {
                std::make_pair("int", (Column){ Type::INT, 0, false, {} }),
                std::make_pair("char", (Column){ Type::CHAR, 2000, false, {} })
          }, std::vector<std::string>({"int", "char"}))
        {}
};

TEST_F(TableTest, saveAndLoad)
{
    table.insert(Table::ColL({std::make_pair("int", "123"), std::make_pair("char", "123")}));
    auto result = table.select({"int", "char"}, Table::ConsL({}));
    ASSERT_THAT(result.size(), Eq(1));
    ASSERT_TRUE(*result[0]["int"] == *Type::newFromLiteral("123", Type::INT));
    ASSERT_TRUE(*result[0]["char"] == *Type::newFromLiteral("123", Type::CHAR, 500));
}

TEST_F(TableTest, remove)
{
    table.insert(Table::ColL({std::make_pair("int", "123")}));
    table.insert(Table::ColL({std::make_pair("int", "456")}));
    table.remove(Table::ConsL({std::make_pair("int", std::vector<Table::ConLiteral>({{Table::EQ, "123"}}))}));
    auto result = table.select({"int"}, Table::ConsL({}));
    ASSERT_THAT(result.size(), Eq(1));
    ASSERT_TRUE(*result[0]["int"] == *Type::newFromLiteral("456", Type::INT));
}

TEST_F(TableTest, largerThan1Page)
{
    for (int i = 0; i < 100; i++)
        table.insert(Table::ColL({std::make_pair("int", std::to_string(i))}));
    table.remove(Table::ConsL({std::make_pair("int", std::vector<Table::ConLiteral>({
        {Table::GE, "10"},
        {Table::LT, "60"}
    }))}));
    auto result = table.select({"int"}, Table::ConsL({std::make_pair("int", std::vector<Table::ConLiteral>({
        {Table::GE, "50"},
        {Table::LT, "80"}
    }))}));
    ASSERT_THAT(result.size(), Eq(20));
}

TEST_F(TableTest, primarySimpleSaveLoad)
{
    tablePri.insert(Table::ColL({std::make_pair("int", "123"), std::make_pair("char", "123")}));
    auto result = tablePri.select({"int", "char"}, Table::ConsL({}));
    ASSERT_THAT(result.size(), Eq(1));
    ASSERT_TRUE(*result[0]["int"] == *Type::newFromLiteral("123", Type::INT));
    ASSERT_TRUE(*result[0]["char"] == *Type::newFromLiteral("123", Type::CHAR, 2000));
}

TEST_F(TableTest, primaryOrderInOnePage)
{
    tablePri.insert(Table::ColL({std::make_pair("int", "1"), std::make_pair("char", "1")}));
    tablePri.insert(Table::ColL({std::make_pair("int", "0"), std::make_pair("char", "0")}));
    tablePri.insert(Table::ColL({std::make_pair("int", "1"), std::make_pair("char", "0")}));
    auto result = tablePri.select({"int", "char"}, Table::ConsL({}));
    ASSERT_THAT(result.size(), Eq(3));
    ASSERT_TRUE(*result[0]["int"] == *Type::newFromLiteral("0", Type::INT));
    ASSERT_TRUE(*result[0]["char"] == *Type::newFromLiteral("0", Type::CHAR, 2000));
    ASSERT_TRUE(*result[1]["int"] == *Type::newFromLiteral("1", Type::INT));
    ASSERT_TRUE(*result[1]["char"] == *Type::newFromLiteral("0", Type::CHAR, 2000));
    ASSERT_TRUE(*result[2]["int"] == *Type::newFromLiteral("1", Type::INT));
    ASSERT_TRUE(*result[2]["char"] == *Type::newFromLiteral("1", Type::CHAR, 2000));
}

TEST_F(TableTest, primaryOrderInMultiplePages)
{
    for (const std::string &item : {"3", "1", "5", "4", "0", "9", "8", "7", "10", "14", "17", "20", "2", "6"})
        tablePri.insert(Table::ColL({std::make_pair("int", item), std::make_pair("char", "")}));
    auto result = tablePri.select({"int", "char"}, Table::ConsL({}));
    ASSERT_THAT(result.size(), Eq(14));
    int rank = 0;
    for (const std::string &item : {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "14", "17", "20"})
    {
        ASSERT_TRUE(*result[rank]["int"] == *Type::newFromLiteral(item, Type::INT))
            << " expect = " << item
            << " actual = " << result[rank]["int"]->toString()
            << std::endl;
        rank++;
    }
}

