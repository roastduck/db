#include "Table.h"

Table::ConsVal Table::genConstraints(const Table::ConsL &literals)
{
    ConsVal ret;
    for (const auto &pair : literals)
    {
        const std::string &name = pair.first;
        for (const auto &literal : pair.second)
        {
            ConValue value;
            value.dir = literal.dir;
            value.pivot = Type::newFromLiteral(literal.pivot, recCols.at(name).typeID, recCols.at(name).length);
            ret[name].push_back(std::move(value));
        }
    }
    return ret;
}

Table::ColVal Table::genVals(const Table::ColL &literals)
{
    ColVal ret;
    for (const auto &pair : literals)
        ret[pair.first] = Type::newFromLiteral(pair.second, recCols.at(pair.first).typeID, recCols.at(pair.first).length);
    return ret;
}

