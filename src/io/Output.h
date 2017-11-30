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

    void printHoriBar(const std::unordered_map<std::string, int> &width);

public:
    Output(std::ostream &_out = std::cout, std::ostream &_err = std::cerr)
        : out(_out), err(_err)
    {}

    void addExcept(const std::exception &e) { err << e.what(); }

    void addResult(const std::vector<Table::ColVal> &result);
};

#endif // OUTPUT_H_
