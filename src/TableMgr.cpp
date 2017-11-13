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
    const Table::Cols &cols,
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
    for (const auto &col : cols)
        sysCols.insert({
            std::make_pair("db", curDb.ok()),
            std::make_pair("table", name),
            std::make_pair("name", col.first),
            std::make_pair("typeID", std::to_string((int)col.second.typeID)),
            std::make_pair("length", std::to_string(col.second.length)),
            std::make_pair("notNull", std::to_string((int)col.second.notNull))
        });
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

    tables[name] = std::unique_ptr<Table>(new Table(cache, curDb.ok() + "." + name, cols, primary, nonClus));
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

void TableMgr::insert(const std::string &tbName, std::vector<Table::ColL> valueLists)
{
    // Check foreign key
    for (const auto &valueList : valueLists)
    {
        for (const auto &foreign : sysForeigns.select({"referrerCols", "referee"}, {
            std::make_pair("db", std::vector<Table::ConLiteral>({{Table::EQ, curDb.ok()}})),
            std::make_pair("referrer", std::vector<Table::ConLiteral>({{Table::EQ, tbName}}))
        }))
        {
            std::vector<std::string> keys;
            const std::string &referrerCols = dynamic_cast<CharType*>(foreign.at("referrerCols").get())->getVal();
            for (const auto &col : commaSep(referrerCols))
            {
                if (!valueList.count(col) || !valueList.at(col).isOk())
                    throw NotNullException(col);
                keys.push_back(valueList.at(col).ok());
            }
            const std::string &referee = dynamic_cast<CharType*>(foreign.at("referee").get())->getVal();
            const Table::Index &refereeCols = tables.at(referee)->getPrimary().ok();
            if (!nameExists(*tables.at(referee), refereeCols, keys))
                throw ForeignKeyViolatedException(tbName, referrerCols, referee, commaJoin(refereeCols));
        }
    }

    // Insert
    for (const auto &valueList : valueLists)
        tables.at(tbName)->insert(valueList);
}

