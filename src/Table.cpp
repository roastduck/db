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
            if (literal.dir != IS_NULL && literal.dir != IS_NOT_NULL)
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
        if (pair.second.isOk())
            ret[pair.first] = Type::newFromLiteral(
                pair.second.ok(), recCols.at(pair.first).typeID, recCols.at(pair.first).length
            );
        else
            ret[pair.first] = nullptr;
    return ret;
}

