#include <algorithm>
#include "TableMgr.h"
#include "type/IntType.h"
#include "type/CharType.h"
#include "exception/NotUniqueException.h"

TableMgr::TableMgr(PageCache &_cache)
    : cache(_cache),
      sysDbs(cache, "$system.dbs", {
            std::make_pair("db", (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true })
      }, Table::Index({"db"})),
      sysTables(cache, "$system.tables", {
            std::make_pair("db", (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair("table", (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
      }, Table::Index({"db", "table"})),
      sysCols(cache, "$system.columns", {
            std::make_pair("db", (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair("table", (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair("name", (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair("typeID", (Column){ Type::INT, 0, true }),
            std::make_pair("length", (Column){ Type::INT, 0, true }),
            std::make_pair("notNull", (Column){ Type::INT, 0, true })
      }, None(), {Table::Index({"db", "table"})}),
      sysPriIdxes(cache, "$system.primaryIndexes", {
            std::make_pair("db", (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair("table", (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair("columns", (Column){ Type::CHAR, (MAX_IDENTIFIER_LEN + 1) * MAX_COLUMN_NUM, true })
      }, Table::Index({"db", "table"})),
      sysNonClusIdxes(cache, "$system.nonClusterIndexes", {
            std::make_pair("db", (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair("table", (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair("columns", (Column){ Type::CHAR, (MAX_IDENTIFIER_LEN + 1) * MAX_COLUMN_NUM, true })
      }, None(), {Table::Index({"db", "table"})}),
      sysForeigns(cache, "$system.foreignKeys", {
            std::make_pair("db", (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair("referrer", (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair("referee", (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair("referrerCols", (Column){ Type::CHAR, (MAX_IDENTIFIER_LEN + 1) * MAX_COLUMN_NUM, true })
      }, None(), {Table::Index({"db", "referrer"}), Table::Index({"db", "referee"})})
{}

bool TableMgr::nameExists(Table &table, const std::vector<std::string> &col, const std::vector<std::string> &name)
{
    assert(col.size() == name.size());
    Table::ConsL cons;
    cons.reserve(col.size());
    for (int i = 0; i < int(col.size()); i++)
        cons[col[i]] = {(Table::ConLiteral){Table::EQ, name[i]}};
    auto result = table.select({}, cons);
    return !result.empty();
}

std::string TableMgr::commaJoin(const std::vector<std::string> &strs)
{
    std::string ret;
    for (const std::string &s : strs)
    {
        if (!ret.empty()) ret.push_back(',');
        ret.append(s);
    }
    return ret;
}

std::vector<std::string> TableMgr::commaSep(const std::string &str)
{
    std::vector<std::string> ret = {""};
    for (char c : str)
        if (c == ',')
            ret.push_back("");
        else
            ret.back().push_back(c);
    return ret;
}

/************************************/
/* DB Managements                   */
/************************************/

void TableMgr::createDb(const std::string &name)
{
    try
    {
        sysDbs.insert({std::make_pair("db", name)});
    } catch (const NotUniqueException &e)
    {
        throw IDAlreadyUsedException("database", name);
    }
}

void TableMgr::use(const std::string &name)
{
    if (!nameExists(sysDbs, {"db"}, {name}))
        throw NoSuchThingException("database", name);
    curDb = name;

    tables.clear();
    for (const auto &tableRec : sysTables.select({"table"}, {
        std::make_pair("db", std::vector<Table::ConLiteral>({{Table::EQ, name}}))
    }))
    {
        // getVal returns reference
        const std::string &table = dynamic_cast<CharType*>(tableRec.at("table").get())->getVal();

        Table::Cols cols;
        for (const auto &colRec : sysCols.select({"name", "typeID", "length", "notNull"}, {
            std::make_pair("db", std::vector<Table::ConLiteral>({{Table::EQ, name}})),
            std::make_pair("table", std::vector<Table::ConLiteral>({{Table::EQ, table}}))
        }))
        {
            const std::string &colName = dynamic_cast<CharType*>(colRec.at("name").get())->getVal();
            Column col;
            col.typeID = (Type::TypeID)dynamic_cast<IntType*>(colRec.at("typeID").get())->getVal();
            col.length = dynamic_cast<IntType*>(colRec.at("length").get())->getVal();
            col.notNull = dynamic_cast<IntType*>(colRec.at("notNull").get())->getVal();
            cols[colName] = std::move(col);
        }

        Optional<Table::Index> primary = None();
        for (const auto &priRec : sysPriIdxes.select({"columns"}, {
            std::make_pair("db", std::vector<Table::ConLiteral>({{Table::EQ, name}})),
            std::make_pair("table", std::vector<Table::ConLiteral>({{Table::EQ, table}}))
        }))
            primary = commaSep(dynamic_cast<CharType*>(priRec.at("columns").get())->getVal());

        std::vector<Table::Index> nonClus;
        for (const auto &nonClusRec : sysNonClusIdxes.select({"columns"}, {
            std::make_pair("db", std::vector<Table::ConLiteral>({{Table::EQ, name}})),
            std::make_pair("table", std::vector<Table::ConLiteral>({{Table::EQ, table}}))
        }))
            nonClus.push_back(commaSep(dynamic_cast<CharType*>(nonClusRec.at("columns").get())->getVal()));

        tables[table] = std::unique_ptr<Table>(new Table(
            cache, name + "." + table, std::move(cols), std::move(primary), std::move(nonClus))
        );
    }
}

void TableMgr::dropDb(const std::string &name)
{
    if (!nameExists(sysDbs, {"db"}, {name}))
        throw NoSuchThingException("database", name);
    sysDbs.remove({std::make_pair("db", std::vector<Table::ConLiteral>({
        {Table::EQ, name}
    }))});
    if (curDb.isOk() && name == curDb.ok())
        curDb = None();
}

std::vector<Table::ColVal> TableMgr::showDbs()
{
    return sysDbs.select({"db"}, {});
}

/************************************/
/* Table Managements                */
/************************************/

void TableMgr::createTable(
    const std::string &name,
    const std::vector< std::pair<std::string, Column> > &cols,
    const Optional<Table::Index> &primary,
    const std::vector<Table::Index> &nonClus,
    const std::vector<TableMgr::ForeignKey> foreigns
)
{
    if (!curDb.isOk())
        throw NoDBInUseException();
    if (nameExists(sysTables, {"db", "table"}, {curDb.ok(), name}))
        throw IDAlreadyUsedException("table", name);
    for (const auto &key : foreigns)
    {
        std::string refereeCols = commaJoin(key.refereeCols);
        std::string primary = "";
        auto result = sysPriIdxes.select({"columns"}, {
            std::make_pair("db", std::vector<Table::ConLiteral>({(Table::ConLiteral){Table::EQ, curDb.ok()}})),
            std::make_pair("table", std::vector<Table::ConLiteral>({(Table::ConLiteral){Table::EQ, key.referee}}))
        });
        if (!result.empty())
            primary = result[0]["columns"]->toString();
        if (refereeCols != primary)
            throw RefereeNotPrimaryException(refereeCols, key.referee, primary);
    }

    sysTables.insert({
        std::make_pair("db", curDb.ok()),
        std::make_pair("table", name)
    });
    Table::Cols colsMap;
    for (const auto &col : cols)
    {
        sysCols.insert({
            std::make_pair("db", curDb.ok()),
            std::make_pair("table", name),
            std::make_pair("name", col.first),
            std::make_pair("typeID", std::to_string((int)col.second.typeID)),
            std::make_pair("length", std::to_string(col.second.length)),
            std::make_pair("notNull", std::to_string((int)col.second.notNull))
        });
        colsMap[col.first] = col.second;
    }
    if (primary.isOk())
        sysPriIdxes.insert({
            std::make_pair("db", curDb.ok()),
            std::make_pair("table", name),
            std::make_pair("columns", commaJoin(primary.ok()))
        });
    for (const auto &idx : nonClus)
        sysNonClusIdxes.insert({
            std::make_pair("db", curDb.ok()),
            std::make_pair("table", name),
            std::make_pair("columns", commaJoin(idx))
        });
    for (const auto &key : foreigns)
        sysForeigns.insert({
            std::make_pair("db", curDb.ok()),
            std::make_pair("referrer", name),
            std::make_pair("referee", key.referee),
            std::make_pair("referrerCols", commaJoin(key.referrerCols))
        });

    tables[name] = std::unique_ptr<Table>(new Table(cache, curDb.ok() + "." + name, colsMap, primary, nonClus));
}

void TableMgr::dropTable(const std::string &name)
{
    if (!curDb.isOk())
        throw NoDBInUseException();
    for (Table *table : {&sysForeigns, &sysNonClusIdxes, &sysPriIdxes, &sysCols, &sysTables})
        table->remove({
            std::make_pair("db", std::vector<Table::ConLiteral>({{Table::EQ, curDb.ok()}})),
            std::make_pair(table == &sysForeigns ? "referrer" : "table", std::vector<Table::ConLiteral>({{Table::EQ, name}}))
        });
    tables.erase(name);
}

std::vector<Table::ColVal> TableMgr::showTables()
{
    if (!curDb.isOk())
        throw NoDBInUseException();
    return sysTables.select({"table"}, {
        std::make_pair("db", std::vector<Table::ConLiteral>({(Table::ConLiteral){Table::EQ, curDb.ok()}})),
    });
}

/************************************/
/* Queries                          */
/************************************/

void TableMgr::insert(const std::string &tbName, const std::vector< std::vector< Optional<std::string> > > &valueLists)
{
    if (!tables.count(tbName))
        throw NoSuchThingException("table", tbName);

    // DO NOT use BaseTable.allColumns because it's out of order
    const auto allColumns = sysCols.select({"name", "notNull"}, {
        std::make_pair("db", std::vector<Table::ConLiteral>({{Table::EQ, curDb.ok()}})),
        std::make_pair("table", std::vector<Table::ConLiteral>({{Table::EQ, tbName}}))
    });

    std::vector<Table::ColL> valueMaps;
    for (const auto &valueList : valueLists)
    {
        if (valueList.size() != allColumns.size())
            throw ValueListLengthNotMatchException(valueList.size(), allColumns.size());
        Table::ColL valueMap;
        for (int i = 0; i < int(valueList.size()); i++)
        {
            const std::string &col = dynamic_cast<CharType*>(allColumns[i].at("name").get())->getVal();
            if (!valueList[i].isOk() && dynamic_cast<IntType*>(allColumns[i].at("notNull").get())->getVal())
                throw NotNullException(col);
            valueMap[col] = valueList[i];
        }
        valueMaps.push_back(std::move(valueMap));
    }

    // Check foreign key
    for (const auto &valueMap : valueMaps)
    {
        for (const auto &foreign : sysForeigns.select({"referrerCols", "referee"}, {
            std::make_pair("db", std::vector<Table::ConLiteral>({{Table::EQ, curDb.ok()}})),
            std::make_pair("referrer", std::vector<Table::ConLiteral>({{Table::EQ, tbName}}))
        }))
        {
            std::vector<std::string> keys;
            const std::string &referrerCols = dynamic_cast<CharType*>(foreign.at("referrerCols").get())->getVal();
            for (const auto &col : commaSep(referrerCols))
                keys.push_back(valueMap.at(col).ok());
            const std::string &referee = dynamic_cast<CharType*>(foreign.at("referee").get())->getVal();
            const Table::Index &refereeCols = tables.at(referee)->getPrimary().ok();
            if (!nameExists(*tables.at(referee), refereeCols, keys))
                throw ForeignKeyViolatedException(tbName, referrerCols, referee, commaJoin(refereeCols));
        }
    }

    // Insert
    for (const auto &valueMap : valueMaps)
        tables.at(tbName)->insert(valueMap);
}

void TableMgr::remove(const std::string &tbName, const Table::ConsL &cons)
{
    if (!tables.count(tbName))
        throw NoSuchThingException("table", tbName);

    // Check foreign key
    const auto &priIdxOpt = tables.at(tbName)->getPrimary();
    if (priIdxOpt.isOk())
    {
        const auto &priIdx = priIdxOpt.ok();
        for (const auto &row : tables.at(tbName)->select(priIdx, cons))
        {
            std::vector<std::string> keys;
            for (const auto &col : priIdx)
                keys.push_back(row.at(col)->toString());
            for (const auto &foreign : sysForeigns.select({"referrerCols", "referrer"}, {
                std::make_pair("db", std::vector<Table::ConLiteral>({{Table::EQ, curDb.ok()}})),
                std::make_pair("referee", std::vector<Table::ConLiteral>({{Table::EQ, tbName}}))
            }))
            {
                const std::string &referrer = dynamic_cast<CharType*>(foreign.at("referrer").get())->getVal();
                const std::string &referrerCols = dynamic_cast<CharType*>(foreign.at("referrerCols").get())->getVal();
                if (nameExists(*tables.at(referrer), commaSep(referrerCols), keys))
                    throw ForeignKeyViolatedException(referrer, referrerCols, tbName, commaJoin(priIdx));
            }
        }
    }

    // Delete
    tables.at(tbName)->remove(cons);
}

void TableMgr::update(const std::string &tbName, const Table::ColL &setClause, const Table::ConsL &cons)
{
    if (!tables.count(tbName))
        throw NoSuchThingException("table", tbName);

    // Check not null
    for (const auto &col : sysCols.select({"name", "notNull"}, {
        std::make_pair("db", std::vector<Table::ConLiteral>({{Table::EQ, curDb.ok()}})),
        std::make_pair("table", std::vector<Table::ConLiteral>({{Table::EQ, tbName}}))
    }))
    {
        const std::string &colName = dynamic_cast<CharType*>(col.at("name").get())->getVal();
        bool notNull = dynamic_cast<IntType*>(col.at("notNull").get())->getVal();
        if (setClause.count(colName) && !setClause.at(colName).isOk() && notNull)
            throw NotNullException(colName);
    }

    // Check foreign key
    const auto &priIdxOpt = tables.at(tbName)->getPrimary();
    const auto oriRows = tables.at(tbName)->select(tables.at(tbName)->getAllColumns(), cons);
    std::vector<Table::ColL> freshRows;
    for (const auto &original : oriRows)
    {
        Table::ColL fresh;
        for (const auto &item : original)
            if (item.second == nullptr)
                fresh[item.first] = None();
            else
                fresh[item.first] = item.second->toString();
        for (const auto &item : setClause)
            fresh[item.first] = item.second;

        // As a referrer
        for (const auto &foreign : sysForeigns.select({"referrerCols", "referee"}, {
            std::make_pair("db", std::vector<Table::ConLiteral>({{Table::EQ, curDb.ok()}})),
            std::make_pair("referrer", std::vector<Table::ConLiteral>({{Table::EQ, tbName}}))
        }))
        {
            std::vector<std::string> keys;
            const std::string &referrerCols = dynamic_cast<CharType*>(foreign.at("referrerCols").get())->getVal();
            for (const auto &col : commaSep(referrerCols))
                keys.push_back(fresh.at(col).ok());
            const std::string &referee = dynamic_cast<CharType*>(foreign.at("referee").get())->getVal();
            const Table::Index &refereeCols = tables.at(referee)->getPrimary().ok();
            if (!nameExists(*tables.at(referee), refereeCols, keys))
                throw ForeignKeyViolatedException(tbName, referrerCols, referee, commaJoin(refereeCols));
        }

        // As a referee
        if (!priIdxOpt.isOk()) continue;
        bool priChanged = false;
        std::vector<std::string> keys;
        for (const auto &col : priIdxOpt.ok())
        {
            keys.push_back(original.at(col)->toString());
            priChanged |= (original.at(col)->toString() != fresh.at(col).ok());
        }
        if (!priChanged) continue;
        for (const auto &foreign : sysForeigns.select({"referrerCols", "referrer"}, {
            std::make_pair("db", std::vector<Table::ConLiteral>({{Table::EQ, curDb.ok()}})),
            std::make_pair("referee", std::vector<Table::ConLiteral>({{Table::EQ, tbName}}))
        }))
        {
            const std::string &referrer = dynamic_cast<CharType*>(foreign.at("referrer").get())->getVal();
            const std::string &referrerCols = dynamic_cast<CharType*>(foreign.at("referrerCols").get())->getVal();
            if (nameExists(*tables.at(referrer), commaSep(referrerCols), keys))
                throw ForeignKeyViolatedException(referrer, referrerCols, tbName, commaJoin(priIdxOpt.ok()));
        }

        freshRows.push_back(std::move(fresh));
    }

    // Update
    tables.at(tbName)->remove(cons);
    for (const auto &row: freshRows)
        tables.at(tbName)->insert(row);
}

std::vector<Table::ColVal> TableMgr::select(
    std::unordered_map< std::string, Table::Index > targets, /// table -> columns
    const std::vector< std::string > &tableList,
    const std::unordered_map< std::string, Table::ConsL > &innerCons, /// table -> constraints
    const OuterConsMap &outterCons
)
{
    for (const auto &name : tableList)
        if (!tables.count(name))
            throw NoSuchThingException("table", name);

    // TODO: optimize the order of `tableList`

    // Complement `targets`
    for (const auto &item : outterCons)
    {
        Table::Index &tb1 = targets[item.first.first], &tb2 = targets[item.first.second];
        for (const auto &out : item.second)
        {
            if (std::find(tb1.begin(), tb1.end(), out.col1) == tb1.end())
                tb1.push_back(out.col1);
            if (std::find(tb2.begin(), tb2.end(), out.col2) == tb2.end())
                tb2.push_back(out.col2);
        }
    }

    std::vector<Table::ColVal> buf1, buf2;
    auto *feed = &buf1, *result = &buf2;
    result->push_back({});
    for (int i = 0; i < int(tables.size()); i++)
    {
        // Gather constraints
        std::vector< std::pair<OuterCon, std::string> > outs; // [(constrints, table)]
        for (int j = 0; j < i; j++)
            for (const auto &pair : {std::make_pair(tableList[i], tableList[j]), std::make_pair(tableList[j], tableList[i])})
                for (auto out : outterCons.count(pair) ? outterCons.at(pair) : OuterCons())
                {
                    if (pair.second == tableList[i])
                    {
                        std::swap(out.col1, out.col2);
                        if (out.dir == Table::LT) out.dir = Table::GT; else
                        if (out.dir == Table::LE) out.dir = Table::GE; else
                        if (out.dir == Table::GT) out.dir = Table::LT; else
                        if (out.dir == Table::GE) out.dir = Table::LE;
                    } // Now `out.col1` is of `tableList[i]`, `out.col2` is of `tableList[j]`
                    outs.push_back(std::make_pair(std::move(out), tableList[j]));
                }

        // Query
        std::swap(feed, result);
        for (auto &line : *feed)
        {
            Table::ConsL cons = innerCons.count(tableList[i]) ? innerCons.at(tableList[i]) : Table::ConsL();
            for (const auto &pair : outs)
            {
                const OuterCon &out = pair.first;
                const std::string &tb2 = pair.second;
                Table::ConLiteral v;
                v.dir = out.dir;
                v.pivot = line.at(tb2 + "." + out.col2)->toString();
                cons[out.col1].push_back(std::move(v));
            }
            auto block = tables.at(tableList[i])->select(
                targets.count(tableList[i]) ? targets.at(tableList[i]) : Table::Index(),
                cons
            );
            for (auto &newLine : block) // Here we destruct `line` and `block`
            {
                result->push_back(std::move(line));
                for (auto &col : newLine)
                    result->back()[tableList[i] + "." + col.first] = std::move(col.second);
            }
        }
        feed->clear();
    }
    return std::move(*result);
}

