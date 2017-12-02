#ifndef BASE_PARSER_H_
#define BASE_PARSER_H_

#include <string>
#include "../Type.h"
#include "../Table.h"
#include "../Column.h"
#include "../TableMgr.h"
#include "../io/Output.h"
#include "antlr4-runtime.h"
#include "../exception/MultiplePrimaryException.h"

class BaseParser : public antlr4::Parser
{
private:
    TableMgr *tableMgr;
    Output *output;

protected:
    BaseParser(antlr4::TokenStream *input)
        : antlr4::Parser(input), tableMgr(NULL) {}

    // We declare some very-short type names used in the grammar file
    typedef std::vector< std::pair<std::string, Column> > Cols;
    typedef Optional< Table::Index > PriIdx;
    typedef std::vector< TableMgr::ForeignKey > Fors;
    typedef std::vector< Optional< std::string > > VList;
    typedef std::vector< VList > VLists;

    // Short helper functions
    template <class T>
    static void append(std::vector<T> &vec, T &item) { vec.push_back(std::move(item)); }

    void showDbs();
    void createDb(const std::string &name);
    void dropDb(const std::string &name);
    void use(const std::string &name);
    void showTables();
    void createTable(const std::string &name, const Cols &cols, const PriIdx &priIdx, const Fors &fors);
    void dropTable(const std::string &name);
    void desc(const std::string &name);
    void insert(const std::string &tbName, const VLists &valueLists);

public:
    void setTableMgr(TableMgr *_tableMgr) { tableMgr = _tableMgr; }
    void setOutput(Output *_output) { output = _output; }
};

#endif // BASE_PARSER_H_
