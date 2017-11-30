#include <cassert>
#include <memory>
#include <algorithm>
#include "Output.h"
#include "../Type.h"

void Output::printHoriBar(const std::unordered_map<std::string, int> &width)
{
    out.fill('-');
    for (const auto &col : width)
        out.width(col.second + 2 * PADDING + 1), out << std::left << '+';
    out << '+' << std::endl;
}

void Output::addResult(const std::vector<Table::ColVal> &result)
{
    if (result.empty())
    {
        out << "(Empty Set)" << std::endl;
        return;
    }

    // Calculate cell width
    std::unordered_map<std::string, int> width;
    for (const auto &row : result)
        for (const auto &col : row)
        {
            assert(!col.first.empty());
            if (col.first.front() == '$') continue;
            int w = std::max(
                width.count(col.first) ? width.at(col.first) : int(col.first.length()), // Header
                int(col.second->toString().length())
            ); // width[col.first] = std::max(...) is wrong, because it will create empty item first
            if (col.second->getTypeID() == Type::INT)
                w = std::max(w, col.second->getLength());
            width[col.first] = w;
        }

    // Add header
    printHoriBar(width);
    out.fill(' ');
    for (const auto &col : width)
    {
        out << '|';
        out.width(PADDING), out << "";
        out.width(col.second + PADDING), out << col.first;
    }
    out << '|' << std::endl;
    printHoriBar(width);

    // Output
    for (const auto &row : result)
    {
        out.fill(' ');
        for (const auto &col : width)
        {
            out << '|';
            out.width(PADDING);
            out << "";
            out.width(col.second + PADDING);
            out << (row.count(col.first) ? row.at(col.first)->toString() : "");
        }
        out << '|' << std::endl;
        printHoriBar(width);
    }
}

