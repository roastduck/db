#include <sstream>
#include "TableMgr.h"
#include "io/Input.h"
#include "io/Output.h"
#include "io/Console.h"
#include "antlr4-runtime.h"

#include "../filesystem/MockPageMgr.h"

#include "gmock/gmock.h"

using namespace testing;

class ConsoleTest : public Test
{
public:
    NiceMock<MockPageMgr> pageMgr;
    PageCache cache;
    TableMgr mgr;
    std::ostringstream outStream, errStream;
    Output output;
    Input input;
    Console console;

    ConsoleTest()
        : cache(pageMgr),
          mgr(cache),
          output(outStream, errStream),
          input(mgr, output),
          console(input)
    {}
};

TEST_F(ConsoleTest, commandInMultipleLines)
{
    console.addCommand("CREATE");
    console.addCommand("DATABASE db;");
    auto dbs = mgr.showDbs();
    ASSERT_THAT(dbs.size(), Eq(1));
    ASSERT_THAT(dbs[0][TableMgr::DB]->toString(), Eq("db"));
}

