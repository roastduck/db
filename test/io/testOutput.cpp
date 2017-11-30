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

    OutputTest() : output(out, err) {}
};

TEST_F(OutputTest, addExcept)
{
    std::runtime_error e("what");
    output.addExcept(e);
    ASSERT_THAT(err.str(), Eq("what"));
}

TEST_F(OutputTest, addResult)
{
    std::vector<Table::ColVal> result(2);
    result[0]["col2"] = Type::newFromLiteral("x", Type::CHAR, 10);
    result[1]["col1"] = Type::newFromLiteral("1", Type::INT);
    result[1]["col2"] = Type::newFromLiteral("foobar", Type::CHAR, 10);
    output.addResult(result);
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

