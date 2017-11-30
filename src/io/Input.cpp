#include "Input.h"
#include "../parser/SqlLexer.h"
#include "../parser/SqlParser.h"

void Input::parse(const std::string &text)
{
    antlr4::ANTLRInputStream charStream(text);
    SqlLexer lexer(&charStream);
    antlr4::CommonTokenStream tokens(&lexer);
    SqlParser parser(&tokens);
    parser.setErrorHandler(Ref<antlr4::BailErrorStrategy>(new antlr4::BailErrorStrategy()));
    parser.setTableMgr(&tableMgr);
    parser.program();
}
