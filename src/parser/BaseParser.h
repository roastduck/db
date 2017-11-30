#ifndef BASE_PARSER_H_
#define BASE_PARSER_H_

#include <string>
#include "../TableMgr.h"
#include "../io/Output.h"
#include "antlr4-runtime.h"

class BaseParser : public antlr4::Parser
{
private:
    TableMgr *tableMgr;
    Output *output;

protected:
    BaseParser(antlr4::TokenStream *input)
        : antlr4::Parser(input), tableMgr(NULL) {}

    void createDb(const std::string &name);

public:
    void setTableMgr(TableMgr *_tableMgr) { tableMgr = _tableMgr; }
    void setOutput(Output *_output) { output = _output; }
};

#endif // BASE_PARSER_H_
