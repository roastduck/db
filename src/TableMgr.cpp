#include <algorithm>
#include "TableMgr.h"
#include "type/IntType.h"
#include "type/CharType.h"
#include "exception/NotUniqueException.h"

constexpr const char *TableMgr::DB;
constexpr const char *TableMgr::TABLE;
constexpr const char *TableMgr::FIELD;
constexpr const char *TableMgr::TYPE;
constexpr const char *TableMgr::LENGTH;
constexpr const char *TableMgr::NOT_NULL;
constexpr const char *TableMgr::IS_PRI;

TableMgr::TableMgr(PageCache &_cache)
    : cache(_cache),
      sysDbs(cache, "$system.dbs", {
            std::make_pair(DB, (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true })
      }, Table::Index({DB})),
      sysTables(cache, "$system.tables", {
            std::make_pair(DB, (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair(TABLE, (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
      }, Table::Index({DB, TABLE})),
      sysCols(cache, "$system.columns", {
            std::make_pair(DB, (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair(TABLE, (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair(FIELD, (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair(TYPE, (Column){ Type::INT, 0, true }),
            std::make_pair(LENGTH, (Column){ Type::INT, 0, true }),
            std::make_pair(NOT_NULL, (Column){ Type::INT, 0, true })
      }, None(), {Table::Index({DB, TABLE})}),
      sysPriIdxes(cache, "$system.primaryIndexes", {
            std::make_pair(DB, (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair(TABLE, (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair("columns", (Column){ Type::CHAR, (MAX_IDENTIFIER_LEN + 1) * MAX_COLUMN_NUM, true })
      }, Table::Index({DB, TABLE})),
      sysNonClusIdxes(cache, "$system.nonClusterIndexes", {
            std::make_pair(DB, (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair(TABLE, (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair("columns", (Column){ Type::CHAR, (MAX_IDENTIFIER_LEN + 1) * MAX_COLUMN_NUM, true }),
            std::make_pair("indexID", (Column){ Type::INT, 0, true })
      }, {Table::Index({DB, TABLE, "columns"})}),
      sysForeigns(cache, "$system.foreignKeys", {
            std::make_pair(DB, (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair("referrer", (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair("referee", (Column){ Type::CHAR, MAX_IDENTIFIER_LEN, true }),
            std::make_pair("referrerCols", (Column){ Type::CHAR, (MAX_IDENTIFIER_LEN + 1) * MAX_COLUMN_NUM, true })
      }, None(), {Table::Index({DB, "referrer"}), Table::Index({DB, "referee"})})
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
        sysDbs.insert({std::make_pair(DB, name)});
    } catch (const NotUniqueException &e)
    {
        throw IDAlreadyUsedException("database", name);
    }
}

void TableMgr::use(const std::string &name)
{
    if (!nameExists(sysDbs, {DB}, {name}))
        throw NoSuchThingException("database", name);
    curDb = name;

    tables.clear();
    for (const auto &tableRec : sysTables.select({TABLE}, {
        std::make_pair(DB, std::vector<Table::ConLiteral>({{Table::EQ, name}}))
    }))
    {
        // getVal returns reference
        const std::string &table = dynamic_cast<CharType*>(tableRec.at(TABLE).get())->getVal();

        Table::Cols cols;
        for (const auto &colRec : sysCols.select({FIELD, TYPE, LENGTH, NOT_NULL}, {
            std::make_pair(DB, std::vector<Table::ConLiteral>({{Table::EQ, name}})),
            std::make_pair(TABLE, std::vector<Table::ConLiteral>({{Table::EQ, table}}))
        }))
        {
            const std::string &colName = dynamic_cast<CharType*>(colRec.at(FIELD).get())->getVal();
            Column col;
            col.typeID = (Type::TypeID)dynamic_cast<IntType*>(colRec.at(TYPE).get())->getVal();
            col.length = dynamic_cast<IntType*>(colRec.at(LENGTH).get())->getVal();
            col.notNull = dynamic_cast<IntType*>(colRec.at(NOT_NULL).get())->getVal();
            cols[colName] = std::move(col);
        }

        Optional<Table::Index> primary = None();
        for (const auto &priRec : sysPriIdxes.select({"columns"}, {
            std::make_pair(DB, std::vector<Table::ConLiteral>({{Table::EQ, name}})),
            std::make_pair(TABLE, std::vector<Table::ConLiteral>({{Table::EQ, table}}))
        }))
            primary = commaSep(dynamic_cast<CharType*>(priRec.at("columns").get())->getVal());

        std::vector<Table::Index> nonClus;
        for (const auto &nonClusRec : sysNonClusIdxes.select({"columns"}, {
            std::make_pair(DB, std::vector<Table::ConLiteral>({{Table::EQ, name}})),
            std::make_pair(TABLE, std::vector<Table::ConLiteral>({{Table::EQ, table}}))
        }))
            nonClus.push_back(commaSep(dynamic_cast<CharType*>(nonClusRec.at("columns").get())->getVal()));

        tables[table] = std::unique_ptr<Table>(new Table(
            cache, name + "." + table, std::move(cols), std::move(primary), std::move(nonClus))
        );
    }
}

void TableMgr::dropDb(const std::string &name)
{
    if (!nameExists(sysDbs, {DB}, {name}))
        throw NoSuchThingException("database", name);
    sysDbs.remove({std::make_pair(DB, std::vector<Table::ConLiteral>({
        {Table::EQ, name}
    }))});
    if (curDb.isOk() && name == curDb.ok())
        curDb = None();
}

std::vector<Table::ColVal> TableMgr::showDbs()
{
    return sysDbs.select({DB}, {});
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
    if (nameExists(sysTables, {DB, TABLE}, {curDb.ok(), name}))
        throw IDAlreadyUsedException(TABLE, name);
    for (const auto &key : foreigns)
    {
        std::string refereeCols = commaJoin(key.refereeCols);
        std::string primary = "";
        auto result = sysPriIdxes.select({"columns"}, {
            std::make_pair(DB, std::vector<Table::ConLiteral>({(Table::ConLiteral){Table::EQ, curDb.ok()}})),
            std::make_pair(TABLE, std::vector<Table::ConLiteral>({(Table::ConLiteral){Table::EQ, key.referee}}))
        });
        if (!result.empty())
            primary = result[0]["columns"]->toString();
        if (refereeCols != primary)
            throw RefereeNotPrimaryException(refereeCols, key.referee, primary);
    }

    sysTables.insert({
        std::make_pair(DB, curDb.ok()),
        std::make_pair(TABLE, name)
    });
    Table::Cols colsMap;
    for (const auto &col : cols)
    {
        sysCols.insert({
            std::make_pair(DB, curDb.ok()),
            std::make_pair(TABLE, name),
            std::make_pair(FIELD, col.first),
            std::make_pair(TYPE, std::to_string((int)col.second.typeID)),
            std::make_pair(LENGTH, std::to_string(col.second.length)),
            std::make_pair(NOT_NULL, std::to_string((int)col.second.notNull))
        });
        colsMap[col.first] = col.second;
    }
    if (primary.isOk())
        sysPriIdxes.insert({
            std::make_pair(DB, curDb.ok()),
            std::make_pair(TABLE, name),
            std::make_pair("columns", commaJoin(primary.ok()))
        });
    int indexID = 0;
    for (const auto &idx : nonClus)
    {
        sysNonClusIdxes.insert({
            std::make_pair(DB, curDb.ok()),
            std::make_pair(TABLE, name),
            std::make_pair("columns", commaJoin(idx)),
            std::make_pair("indexID", std::to_string(indexID))
        });
        indexID++;
    }
    for (const auto &key : foreigns)
        sysForeigns.insert({
            std::make_pair(DB, curDb.ok()),
            std::make_pair("referrer", name),
            std::make_pair("referee", key.referee),
            std::make_pair("referrerCols", commaJoin(key.referrerCols))
        });

    tables[name] = std::unique_ptr<Table>(new Table(cache, curDb.ok() + "." + name, colsMap, primary, nonClus));
}

void TableMgr::dropTable(const std::string &name)
{
    if (!tables.count(name))
        throw NoSuchThingException(TABLE, name);
    for (Table *table : {&sysForeigns, &sysNonClusIdxes, &sysPriIdxes, &sysCols, &sysTables})
        table->remove({
            std::make_pair(DB, std::vector<Table::ConLiteral>({{Table::EQ, curDb.ok()}})),
            std::make_pair(table == &sysForeigns ? "referrer" : TABLE, std::vector<Table::ConLiteral>({{Table::EQ, name}}))
        });
    tables.erase(name);
}

std::vector<Table::ColVal> TableMgr::showTables()
{
    if (!curDb.isOk())
        throw NoDBInUseException();
    return sysTables.select({TABLE}, {
        std::make_pair(DB, std::vector<Table::ConLiteral>({(Table::ConLiteral){Table::EQ, curDb.ok()}})),
    });
}

std::vector<Table::ColVal> TableMgr::desc(const std::string &name)
{
    if (!tables.count(name))
        throw NoSuchThingException(TABLE, name);
    auto result = sysCols.select({FIELD, TYPE, LENGTH, NOT_NULL}, {
        std::make_pair(DB, std::vector<Table::ConLiteral>({(Table::ConLiteral){Table::EQ, curDb.ok()}})),
        std::make_pair(TABLE, std::vector<Table::ConLiteral>({(Table::ConLiteral){Table::EQ, name}}))
    });
    const auto &priIdx = tables.at(name)->getPrimary();
    for (auto &row : result)
    {
        const std::string &field = dynamic_cast<CharType*>(row.at(FIELD).get())->getVal();
        std::string typeStr = Type::getName(Type::TypeID(dynamic_cast<IntType*>(row.at(TYPE).get())->getVal()));
        int length = dynamic_cast<IntType*>(row.at(LENGTH).get())->getVal();
        bool notNull = dynamic_cast<IntType*>(row.at(NOT_NULL).get())->getVal();
        if (length > 0)
            typeStr += "(" + std::to_string(length) + ")";
        row.at(TYPE) = Type::newFromLiteral(typeStr, Type::CHAR, typeStr.length());
        row.at(NOT_NULL) = Type::newFromLiteral(notNull ? "YES" : "NO", Type::CHAR, 3);
        bool isPri = priIdx.isOk() && std::find(priIdx.ok().begin(), priIdx.ok().end(), field) != priIdx.ok().end();
        row[IS_PRI] = Type::newFromLiteral(isPri ? "YES" : "NO", Type::CHAR, 3);
        row.erase(LENGTH);
    }
    return result;
}

/************************************/
/* Index Managements                */
/************************************/

void TableMgr::createIndex(const std::string &tbName, const Table::Index &colName)
{
    if (!tables.count(tbName))
        throw NoSuchThingException(TABLE, tbName);

    int indexID = tables.at(tbName)->addIndex(colName);
    sysNonClusIdxes.insert({
        std::make_pair(DB, curDb.ok()),
        std::make_pair(TABLE, tbName),
        std::make_pair("columns", commaJoin(colName)),
        std::make_pair("indexID", std::to_string(indexID))
    });
}

void TableMgr::dropIndex(const std::string &tbName, const Table::Index &colName)
{
    if (!tables.count(tbName))
        throw NoSuchThingException(TABLE, tbName);

    auto result = sysNonClusIdxes.select({"indexID"}, {
        std::make_pair(DB, std::vector<Table::ConLiteral>({{Table::EQ, curDb.ok()}})),
        std::make_pair(TABLE, std::vector<Table::ConLiteral>({{Table::EQ, tbName}})),
        std::make_pair("column", std::vector<Table::ConLiteral>({{Table::EQ, commaJoin(colName)}}))
    });
    if (result.empty())
        throw NoSuchThingException("index", "(" + commaJoin(colName) + ")");
    assert(result.size() == 1);
    auto indexID = dynamic_cast<IntType*>(result[0].at("column").get())->getVal();
    tables.at(tbName)->delIndex(indexID);
}

/************************************/
/* Queries                          */
/************************************/

void TableMgr::insert(const std::string &tbName, const std::vector< std::vector< Optional<std::string> > > &valueLists)
{
    if (!tables.count(tbName))
        throw NoSuchThingException(TABLE, tbName);

    // DO NOT use BaseTable.allColumns because it's out of order
    const auto allColumns = sysCols.select({FIELD, NOT_NULL}, {
        std::make_pair(DB, std::vector<Table::ConLiteral>({{Table::EQ, curDb.ok()}})),
        std::make_pair(TABLE, std::vector<Table::ConLiteral>({{Table::EQ, tbName}}))
    });

    std::vector<Table::ColL> valueMaps;
    for (const auto &valueList : valueLists)
    {
        if (valueList.size() != allColumns.size())
            throw ValueListLengthNotMatchException(valueList.size(), allColumns.size());
        Table::ColL valueMap;
        for (int i = 0; i < int(valueList.size()); i++)
        {
            const std::string &col = dynamic_cast<CharType*>(allColumns[i].at(FIELD).get())->getVal();
            if (!valueList[i].isOk() && dynamic_cast<IntType*>(allColumns[i].at(NOT_NULL).get())->getVal())
                throw NotNullException(col);
            valueMap[col] = valueList[i];
        }
        valueMaps.push_back(std::move(valueMap));
    }

    // Check foreign key
    for (const auto &valueMap : valueMaps)
    {
        for (const auto &foreign : sysForeigns.select({"referrerCols", "referee"}, {
            std::make_pair(DB, std::vector<Table::ConLiteral>({{Table::EQ, curDb.ok()}})),
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

void TableMgr::remove(const std::string &tbName, const Table::ConsL &cons, const Table::OuterCons &oCons)
{
    if (!tables.count(tbName))
        throw NoSuchThingException(TABLE, tbName);

    // Check foreign key
    const auto &priIdxOpt = tables.at(tbName)->getPrimary();
    if (priIdxOpt.isOk())
    {
        const auto &priIdx = priIdxOpt.ok();
        for (const auto &row : tables.at(tbName)->select(priIdx, cons, oCons))
        {
            std::vector<std::string> keys;
            for (const auto &col : priIdx)
                keys.push_back(row.at(col)->toString());
            for (const auto &foreign : sysForeigns.select({"referrerCols", "referrer"}, {
                std::make_pair(DB, std::vector<Table::ConLiteral>({{Table::EQ, curDb.ok()}})),
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
    tables.at(tbName)->remove(cons, oCons);
}

void TableMgr::update(
    const std::string &tbName, const Table::ColL &setClause, const Table::ConsL &cons, const Table::OuterCons &oCons
)
{
    if (!tables.count(tbName))
        throw NoSuchThingException(TABLE, tbName);

    // Check not null
    for (const auto &col : sysCols.select({FIELD, NOT_NULL}, {
        std::make_pair(DB, std::vector<Table::ConLiteral>({{Table::EQ, curDb.ok()}})),
        std::make_pair(TABLE, std::vector<Table::ConLiteral>({{Table::EQ, tbName}}))
    }))
    {
        const std::string &colName = dynamic_cast<CharType*>(col.at(FIELD).get())->getVal();
        bool notNull = dynamic_cast<IntType*>(col.at(NOT_NULL).get())->getVal();
        if (setClause.count(colName) && !setClause.at(colName).isOk() && notNull)
            throw NotNullException(colName);
    }

    // Check foreign key
    const auto &priIdxOpt = tables.at(tbName)->getPrimary();
    const auto oriRows = tables.at(tbName)->select(tables.at(tbName)->getAllColumns(), cons, oCons);
    std::vector<Table::ColL> freshRows;
    for (const auto &original : oriRows)
    {
        Table::ColL fresh;
        for (const auto &item : original)
            if (item.first[0] != '$')
            {
                if (item.second == nullptr)
                    fresh[item.first] = None();
                else
                    fresh[item.first] = item.second->toString();
            }
        for (const auto &item : setClause)
            fresh[item.first] = item.second;
        freshRows.push_back(fresh);

        // As a referrer
        for (const auto &foreign : sysForeigns.select({"referrerCols", "referee"}, {
            std::make_pair(DB, std::vector<Table::ConLiteral>({{Table::EQ, curDb.ok()}})),
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
            std::make_pair(DB, std::vector<Table::ConLiteral>({{Table::EQ, curDb.ok()}})),
            std::make_pair("referee", std::vector<Table::ConLiteral>({{Table::EQ, tbName}}))
        }))
        {
            const std::string &referrer = dynamic_cast<CharType*>(foreign.at("referrer").get())->getVal();
            const std::string &referrerCols = dynamic_cast<CharType*>(foreign.at("referrerCols").get())->getVal();
            if (nameExists(*tables.at(referrer), commaSep(referrerCols), keys))
                throw ForeignKeyViolatedException(referrer, referrerCols, tbName, commaJoin(priIdxOpt.ok()));
        }
    }

    // Update
    tables.at(tbName)->remove(cons, oCons);
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
            throw NoSuchThingException(TABLE, name);

    // TODO: optimize the order of `tableList`

    // Complement `targets`
    for (const auto &item : outterCons)
    {
        if (item.first.first == item.first.second) continue;
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
        std::vector< std::pair<Table::OuterCon, std::string> > outs; // [(constrints, table)]
        for (int j = 0; j < i; j++)
            for (const auto &pair : {std::make_pair(tableList[i], tableList[j]), std::make_pair(tableList[j], tableList[i])})
                for (auto out : outterCons.count(pair) ? outterCons.at(pair) : Table::OuterCons())
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
        for (const auto &line : *feed)
        {
            Table::ConsL cons = innerCons.count(tableList[i]) ? innerCons.at(tableList[i]) : Table::ConsL();
            Table::OuterCons oCons;
            if (outterCons.count(std::make_pair(tableList[i], tableList[i])))
                oCons = outterCons.at(std::make_pair(tableList[i], tableList[i]));
            for (const auto &pair : outs)
            {
                const Table::OuterCon &out = pair.first;
                const std::string &tb2 = pair.second;
                Table::ConLiteral v;
                v.dir = out.dir;
                v.pivot = line.at(tb2 + "." + out.col2)->toString();
                cons[out.col1].push_back(std::move(v));
            }
            auto block = tables.at(tableList[i])->select(
                targets.count(tableList[i]) ? targets.at(tableList[i]) : Table::Index(),
                cons,
                oCons
            );
            for (auto &newLine : block) // Here we destruct `block`
            {
                // We can't std::move(line), because 1 line -> multiple newLine
                result->push_back({});
                for (const auto &col : line)
                    result->back()[col.first] = Type::newFromCopy(col.second);
                for (auto &col : newLine)
                    result->back()[tableList[i] + "." + col.first] = std::move(col.second);
            }
        }
        feed->clear();
    }
    return std::move(*result);
}

