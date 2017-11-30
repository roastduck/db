#include <cassert>
#include <memory>
#include <algorithm>
#include "Output.h"
#include "../Type.h"

void Output::printHoriBar(const std::unordered_map<std::string, int> &width, const Table::Index &order)
{
    out.fill('-');
    for (const auto &col : order)
        out.width(width.at(col) + 2 * PADDING + 1), out << std::left << '+';
    out << '+' << std::endl;
}

void Output::addResult(const std::vector<Table::ColVal> &result, Table::Index order)
{
    if (result.empty())
    {
        out << "(Empty set)" << std::endl;
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
    if (order.empty())
        for (const auto &col : width)
            order.push_back(col.first);
    assert(order.size() == width.size());
    printHoriBar(width, order);
    out.fill(' ');
    for (const auto &col : order)
    {
        out << '|';
        out.width(PADDING), out << "";
        out.width(width.at(col) + PADDING), out << col;
    }
    out << '|' << std::endl;
    printHoriBar(width, order);

    // Output
    for (const auto &row : result)
    {
        out.fill(' ');
        for (const auto &col : order)
        {
            out << '|';
            out.width(PADDING);
            out << "";
            out.width(width.at(col) + PADDING);
            out << (row.count(col) ? row.at(col)->toString() : "");
        }
        out << '|' << std::endl;
        printHoriBar(width, order);
    }
}

