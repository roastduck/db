#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include <unordered_map>
#include "../Table.h"

class Output
{
private:
    std::ostream &out, &info, &err;
    bool csvMode;

    const int PADDING = 1; /// Number of spaces around the text in a table cell

    void printHoriBar(const std::unordered_map<std::string, int> &width, const Table::Index &order);

    void addResultConsole(
        const std::vector<Table::ColVal> &result,
        Table::Index order,
        const std::unordered_map<std::string, int> &width
    );
    void addResultCsv(const std::vector<Table::ColVal> &result, Table::Index order);

public:
    Output(std::ostream &_out = std::cout, std::ostream &_info = std::clog, std::ostream &_err = std::cerr)
        : out(_out), info(_info), err(_err), csvMode(false)
    {}

    void addExcept(const std::exception &e) { err << e.what() << std::endl; }

    void addInfo(const std::string &infomation) { info << infomation << std::endl; }

    /** Display a table
     *  @param order : Optionally specify the order of columns to be output
     */
    void addResult(const std::vector<Table::ColVal> &result, Table::Index order = {});

    void setCsvMode(bool _csvMode) { csvMode = _csvMode; }
};

#endif // OUTPUT_H_
