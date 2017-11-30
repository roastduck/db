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
    std::ostream &out, &err;

    const int PADDING = 1; /// Number of spaces around the text in a table cell

    void printHoriBar(const std::unordered_map<std::string, int> &width, const Table::Index &order);

public:
    Output(std::ostream &_out = std::cout, std::ostream &_err = std::cerr)
        : out(_out), err(_err)
    {}

    void addExcept(const std::exception &e) { err << e.what() << std::endl; }

    void addInfo(const std::string &info) { out << info << std::endl; }

    /** Display a table
     *  @param order : Optionally specify the order of columns to be output
     */
    void addResult(const std::vector<Table::ColVal> &result, Table::Index order = {});
};

#endif // OUTPUT_H_
