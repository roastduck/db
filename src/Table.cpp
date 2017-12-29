#include "Table.h"

Table::ConsVal Table::genConstraints(const Table::ConsL &literals) const
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
                try
                {
                    value.pivot = Type::newFromLiteral(literal.pivot, recCols.at(name).typeID, recCols.at(name).length);
                } catch (const std::out_of_range &e)
                {
                    throw NoSuchThingException("field", name);
                }
            ret[name].push_back(std::move(value));
        }
    }
    return ret;
}

Table::ColVal Table::genVals(const Table::ColL &literals) const
{
    ColVal ret;
    for (const auto &pair : literals)
        if (pair.second.isOk())
            try
            {
                ret[pair.first] = Type::newFromLiteral(
                    pair.second.ok(), recCols.at(pair.first).typeID, recCols.at(pair.first).length
                );
            } catch (const std::out_of_range &e)
            {
                throw NoSuchThingException("field", pair.first);
            }
        else
            ret[pair.first] = nullptr;
    return ret;
}

