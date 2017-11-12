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
    Table table, tablePri, tableNon, tableBoth, tableBothWithLargePri;

    TableTest()
        : cache(pageMgr),
          table(cache, "table", {
                std::make_pair("int", (Column){ Type::INT, 0, false }),
                std::make_pair("char", (Column){ Type::CHAR, 500, false })
          }),
          tablePri(cache, "tablePri", {
                std::make_pair("int", (Column){ Type::INT, 0, false }),
                std::make_pair("char", (Column){ Type::CHAR, 2000, false })
          }, Table::Index({"int", "char"})),
          tableNon(cache, "tableNon", {
                std::make_pair("int", (Column){ Type::INT, 0, false }),
                std::make_pair("char", (Column){ Type::CHAR, 2000, false })
          }, None(), {Table::Index({"int"}), Table::Index({"char"})}),
          tableBoth(cache, "tableBoth", {
                std::make_pair("int1", (Column){ Type::INT, 0, false }),
                std::make_pair("int2", (Column){ Type::INT, 0, false }),
          }, Table::Index({"int1"}), {Table::Index({"int2"})}),
          tableBothWithLargePri(cache, "tableBothWithLargePri", {
                std::make_pair("int1", (Column){ Type::CHAR, 2000, false }),
                std::make_pair("int2", (Column){ Type::INT, 0, false }),
                std::make_pair("payload", (Column){ Type::INT, 0, false })
          }, Table::Index({"int1"}), {Table::Index({"int2"})})
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

TEST_F(TableTest, primarySelect)
{
    for (int i = 0; i < 20; i++)
        tablePri.insert(Table::ColL({std::make_pair("int", std::to_string(i)), std::make_pair("char", "")}));
    auto result = tablePri.select({"int"}, Table::ConsL({std::make_pair("int", std::vector<Table::ConLiteral>({
        {Table::GE, "3"},
        {Table::LT, "16"}
    }))}));
    ASSERT_THAT(result.size(), Eq(13));
}

TEST_F(TableTest, primarySelectCombinational)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 10; j++)
            tablePri.insert(Table::ColL({std::make_pair("int", std::to_string(i)), std::make_pair("char", std::to_string(j))}));
    auto result = tablePri.select({"int"}, Table::ConsL({
        std::make_pair("int", std::vector<Table::ConLiteral>({
            {Table::EQ, "1"}
        })),
        std::make_pair("char", std::vector<Table::ConLiteral>({
            {Table::GT, "1"},
            {Table::LE, "7"}
        }))
    }));
    ASSERT_THAT(result.size(), Eq(6));
}

TEST_F(TableTest, primaryRemoveOnePage)
{
    tablePri.insert(Table::ColL({std::make_pair("int", "0"), std::make_pair("char", "")}));
    tablePri.insert(Table::ColL({std::make_pair("int", "1"), std::make_pair("char", "")}));
    tablePri.insert(Table::ColL({std::make_pair("int", "2"), std::make_pair("char", "")}));
    tablePri.remove(Table::ConsL({std::make_pair("int", std::vector<Table::ConLiteral>({
        {Table::EQ, "1"}
    }))}));
    auto result = tablePri.select({"int"}, Table::ConsL({}));
    ASSERT_THAT(result.size(), Eq(2));
    ASSERT_TRUE(*result[0]["int"] == *Type::newFromLiteral("0", Type::INT));
    ASSERT_TRUE(*result[1]["int"] == *Type::newFromLiteral("2", Type::INT));
}

TEST_F(TableTest, primaryRemoveMultiplePages)
{
    for (int i = 0; i < 20; i++)
        tablePri.insert(Table::ColL({std::make_pair("int", std::to_string(i)), std::make_pair("char", "")}));
    tablePri.remove(Table::ConsL({std::make_pair("int", std::vector<Table::ConLiteral>({
        {Table::GE, "5"},
        {Table::LT, "16"}
    }))}));
    auto result = tablePri.select({"int"}, Table::ConsL({}));
    for (int i = 0; i < 5; i++)
        ASSERT_TRUE(*result[i]["int"] == *Type::newFromLiteral(std::to_string(i), Type::INT))
            << " expect = " << i
            << " actual = " << result[i]["int"]->toString();
    for (int i = 0; i < 4; i++)
        ASSERT_TRUE(*result[5 + i]["int"] == *Type::newFromLiteral(std::to_string(16 + i), Type::INT))
            << " expect = " << 16 + i
            << " actual = " << result[5 + i]["int"]->toString();
    ASSERT_THAT(result.size(), Eq(9));
}

TEST_F(TableTest, nonClusterSelectOnePage)
{
    for (int i = 0; i < 20; i++)
        tableNon.insert(Table::ColL({std::make_pair("int", std::to_string(i)), std::make_pair("char", "")}));
    auto result = tableNon.select({"int"}, Table::ConsL({std::make_pair("int", std::vector<Table::ConLiteral>({
        {Table::GE, "3"},
        {Table::LT, "16"}
    }))}));
    ASSERT_THAT(result.size(), Eq(13));
}

TEST_F(TableTest, nonClusterSelectMultipleIdxPages)
{
    for (int i = 10; i < 30; i++) // NOTE: "3" > "10"
        tableNon.insert(Table::ColL({std::make_pair("int", "0"), std::make_pair("char", std::to_string(i))}));
    auto result = tableNon.select({"char"}, Table::ConsL({std::make_pair("char", std::vector<Table::ConLiteral>({
        {Table::GE, "13"},
        {Table::LT, "26"}
    }))}));
    ASSERT_THAT(result.size(), Eq(13));
}

TEST_F(TableTest, nonClusterRemove)
{
    tableNon.insert(Table::ColL({std::make_pair("int", "0"), std::make_pair("char", "0")}));
    tableNon.insert(Table::ColL({std::make_pair("int", "1"), std::make_pair("char", "0")}));
    tableNon.insert(Table::ColL({std::make_pair("int", "1"), std::make_pair("char", "1")}));
    tableNon.insert(Table::ColL({std::make_pair("int", "1"), std::make_pair("char", "2")}));
    tableNon.insert(Table::ColL({std::make_pair("int", "1"), std::make_pair("char", "3")}));
    tableNon.insert(Table::ColL({std::make_pair("int", "1"), std::make_pair("char", "4")}));
    tableNon.insert(Table::ColL({std::make_pair("int", "1"), std::make_pair("char", "5")}));
    tableNon.insert(Table::ColL({std::make_pair("int", "1"), std::make_pair("char", "6")}));

    tableNon.remove(Table::ConsL({std::make_pair("int", std::vector<Table::ConLiteral>({
            {Table::EQ, "1"},
        })),
        std::make_pair("char", std::vector<Table::ConLiteral>({
            {Table::LE, "5"},
        }))
    }));

    auto result = tableNon.select({"char"}, Table::ConsL({}));
    ASSERT_THAT(result.size(), Eq(2));

    result = tableNon.select({"char"}, Table::ConsL({std::make_pair("int", std::vector<Table::ConLiteral>({
        {Table::EQ, "1"},
    }))}));
    ASSERT_THAT(result.size(), Eq(1));
    ASSERT_TRUE(*result[0]["char"] == *Type::newFromLiteral("6", Type::CHAR, 2000));
}

TEST_F(TableTest, bothIndexSelect)
{
    for (int i = 0; i < 30; i++)
        tableBoth.insert(Table::ColL({
            std::make_pair("int1", std::to_string(30 - i)), std::make_pair("int2", std::to_string(i))
        }));
    auto result = tableBoth.select({"int1", "int2"}, Table::ConsL({std::make_pair("int2", std::vector<Table::ConLiteral>({
        {Table::GE, "7"},
        {Table::LT, "24"}
    }))}));
    ASSERT_THAT(result.size(), Eq(17));
    for (int i = 0, j = 7; i < 17; i++, j++)
    {
        ASSERT_TRUE(*result[i]["int2"] == *Type::newFromLiteral(std::to_string(j), Type::INT))
            << " int2: "
            << " expect = " << j
            << " actual = " << result[i]["int2"]->toString();
        ASSERT_TRUE(*result[i]["int1"] == *Type::newFromLiteral(std::to_string(30 - j), Type::INT))
            << " int1: "
            << " expect = " << 30 - j
            << " actual = " << result[i]["int1"]->toString();
    }
}

TEST_F(TableTest, bothIndexRemove)
{
    tableBoth.insert(Table::ColL({std::make_pair("int1", "-1"), std::make_pair("int2", "0")}));
    tableBoth.insert(Table::ColL({std::make_pair("int1", "0"), std::make_pair("int2", "1")}));
    tableBoth.insert(Table::ColL({std::make_pair("int1", "1"), std::make_pair("int2", "1")}));
    tableBoth.insert(Table::ColL({std::make_pair("int1", "2"), std::make_pair("int2", "1")}));
    tableBoth.insert(Table::ColL({std::make_pair("int1", "3"), std::make_pair("int2", "1")}));
    tableBoth.insert(Table::ColL({std::make_pair("int1", "4"), std::make_pair("int2", "1")}));
    tableBoth.insert(Table::ColL({std::make_pair("int1", "5"), std::make_pair("int2", "1")}));
    tableBoth.insert(Table::ColL({std::make_pair("int1", "6"), std::make_pair("int2", "1")}));

    tableBoth.remove(Table::ConsL({std::make_pair("int2", std::vector<Table::ConLiteral>({
            {Table::EQ, "1"},
        })),
        std::make_pair("int1", std::vector<Table::ConLiteral>({
            {Table::LE, "5"},
        }))
    }));

    auto result = tableBoth.select({"int1"}, Table::ConsL({}));
    ASSERT_THAT(result.size(), Eq(2));

    result = tableBoth.select({"int1"}, Table::ConsL({std::make_pair("int2", std::vector<Table::ConLiteral>({
        {Table::EQ, "1"},
    }))}));
    ASSERT_THAT(result.size(), Eq(1));
    ASSERT_TRUE(*result[0]["int1"] == *Type::newFromLiteral("6", Type::INT));
}

TEST_F(TableTest, bothIndexManyInRefPage)
{
    for (int i = 0; i < 70; i++)
        tableBothWithLargePri.insert(Table::ColL({
            std::make_pair("int1", std::to_string(i)),
            std::make_pair("int2", "0"),
            std::make_pair("payload", std::to_string(i)),
        }));
    tableBothWithLargePri.remove(Table::ConsL({std::make_pair("int2", std::vector<Table::ConLiteral>({
            {Table::EQ, "0"},
        })),
        std::make_pair("payload", std::vector<Table::ConLiteral>({
            {Table::GE, "10"},
            {Table::LT, "55"}
        }))
    }));
    auto result = tableBothWithLargePri.select({"int1"}, Table::ConsL({std::make_pair("int2", std::vector<Table::ConLiteral>({
        {Table::EQ, "0"},
    }))}));
    ASSERT_THAT(result.size(), Eq(25));
}

TEST_F(TableTest, bothIndexManyInRefPageRemovingHead)
{
    for (int i = 0; i < 70; i++)
        tableBothWithLargePri.insert(Table::ColL({
            std::make_pair("int1", std::to_string(i)),
            std::make_pair("int2", "0"),
            std::make_pair("payload", std::to_string(i)),
        }));
    tableBothWithLargePri.remove(Table::ConsL({std::make_pair("int2", std::vector<Table::ConLiteral>({
            {Table::EQ, "0"},
        })),
        std::make_pair("payload", std::vector<Table::ConLiteral>({
            {Table::GE, "0"}, // 0 is the head
            {Table::LT, "55"}
        }))
    }));
    auto result = tableBothWithLargePri.select({"int1"}, Table::ConsL({std::make_pair("int2", std::vector<Table::ConLiteral>({
        {Table::EQ, "0"},
    }))}));
    ASSERT_THAT(result.size(), Eq(15));
}

