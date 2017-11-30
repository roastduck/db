#include "Input.h"
#include "../parser/SqlLexer.h"
#include "../parser/SqlParser.h"
#include "../exception/SyntaxError.h"

void Input::parse(antlr4::CharStream &charStream)
{
    SqlLexer lexer(&charStream);
    antlr4::CommonTokenStream tokens(&lexer);
    SqlParser parser(&tokens);
    parser.setErrorHandler(Ref<antlr4::BailErrorStrategy>(new antlr4::BailErrorStrategy()));
    parser.setTableMgr(&tableMgr);
    try
    {
        try
        {
            parser.program();
        } catch (const antlr4::ParseCancellationException &e)
        {
            throw SyntaxError();
        }
    } catch (const std::exception &e)
    {
        output.addExcept(e);
    }
}

void Input::parse(const std::string &text)
{
    antlr4::ANTLRInputStream charStream(text);
    parse(charStream);
}

