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

void Table::insert(const std::vector<Table::ColL> &literals)
{
    // Convert to values
    std::vector<ColVal> values;
    values.reserve(literals.size());
    for (const auto lit : literals)
    {
        auto vals = genVals(lit);
        for (const auto &col : recCols)
            if (!vals.count(col.first))
                vals[col.first] = nullptr;
        values.push_back(std::move(vals));
    }

    // Pre-sort
    if (getPrimary().isOk())
        sort(values.begin(), values.end(), getPrimary().ok());

    // Insert
    for (int i = 0; i < int(values.size()); i++)
        try
        {
            BaseTable::insert(values[i]);
        } catch (const NotUniqueException &e)
        {
            // Handling NotUniqueException, don't worring about duplicate deletion
            for (int j = 0; j < i; j++)
            {
                ConsVal cons;
                cons.reserve(values[j].size());
                for (const auto &col : values[j])
                    cons[col.first].push_back((ConValue){
                        col.second == nullptr ? Table::IS_NULL : Table::EQ,
                        Type::newFromCopy(col.second)
                    });
                BaseTable::remove(std::move(cons), {});
            }
            throw e;
        }
}

void Table::insert(const ColL &literals)
{
    auto vals = genVals(literals);
    for (const auto &col : recCols)
        if (!vals.count(col.first))
            vals[col.first] = nullptr;
    BaseTable::insert(vals);
}

