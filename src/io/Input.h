#ifndef INPUT_H_
#define INPUT_H_

#include <string>
#include "Output.h"
#include "../TableMgr.h"
#include "antlr4-runtime.h"

class Input
{
private:
    TableMgr &tableMgr;
    Output &output;

    /** Parse a char stream
     *  All exceptions defined in exception/ can be thrown from this method
     */
    void parse(antlr4::CharStream &charStream);

public:
    Input(TableMgr &_tableMgr, Output &_output)
        : tableMgr(_tableMgr), output(_output)
    {}

    /** Parse a string
     *  This string must have a complete sytax
     */
    void parse(const std::string &text);
};

#endif // INPUT_H_
