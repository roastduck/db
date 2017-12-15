#include <sstream>
#include <stdexcept>
#include "Type.h"
#include "Table.h"
#include "io/Output.h"

#include "gmock/gmock.h"

using namespace testing;

class OutputTest : public Test
{
public:
    std::ostringstream out, err;
    Output output;

    OutputTest() : output(out, out, err) {}
};

TEST_F(OutputTest, addExcept)
{
    std::runtime_error e("what");
    output.addExcept(e);
    ASSERT_THAT(err.str(), Eq("what\n"));
}

TEST_F(OutputTest, addInfo)
{
    output.addInfo("info");
    ASSERT_THAT(out.str(), Eq("info\n"));
}

TEST_F(OutputTest, addResult)
{
    std::vector<Table::ColVal> result(2);
    result[0]["col2"] = Type::newFromLiteral("x", Type::CHAR, 10);
    result[1]["col1"] = Type::newFromLiteral("1", Type::INT);
    result[1]["col2"] = Type::newFromLiteral("foobar", Type::CHAR, 10);
    output.addResult(result, {"col1", "col2"});
    ASSERT_THAT(out.str(), Eq(
        "+------+--------+\n"
        "| col1 | col2   |\n"
        "+------+--------+\n"
        "|      | x      |\n"
        "+------+--------+\n"
        "| 1    | foobar |\n"
        "+------+--------+\n"
    ));
}

TEST_F(OutputTest, lengthOptionInIntType)
{
    std::vector<Table::ColVal> result(1);
    result[0]["c"] = Type::newFromLiteral("0", Type::INT, 5);
    output.addResult(result);
    ASSERT_THAT(out.str(), Eq(
        "+-------+\n"
        "| c     |\n"
        "+-------+\n"
        "| 0     |\n" // length 5 + 2 padding
        "+-------+\n"
    ));
}

TEST_F(OutputTest, csvMode)
{
    output.setCsvMode(true);
    std::vector<Table::ColVal> result(2);
    result[0]["col2"] = Type::newFromLiteral("x", Type::CHAR, 10);
    result[1]["col1"] = Type::newFromLiteral("1", Type::INT);
    result[1]["col2"] = Type::newFromLiteral("foobar", Type::CHAR, 10);
    output.addResult(result, {"col1", "col2"});
    ASSERT_THAT(out.str(), Eq(
        "col1,col2\n"
        ",x\n"
        "1,foobar\n"
        "\n"
    ));
}

