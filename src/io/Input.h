#ifndef INPUT_H_
#define INPUT_H_

#include <string>
#include "../TableMgr.h"

class Input
{
private:
    TableMgr &tableMgr;

public:
    Input(TableMgr &_tableMgr) : tableMgr(_tableMgr) {}

    /** Parse a string
     *  This string must have a complete sytax
     *  @throw antlr4::ParseCancellationException
     *  and any exceptions thrown by TableMgr
     */
    void parse(const std::string &text);
};

#endif // INPUT_H_
