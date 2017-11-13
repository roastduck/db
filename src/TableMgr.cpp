#include "TableMgr.h"
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
      }, Table::Index({"db", "referrer", "referrerCols"}), {Table::Index({"db", "referee"})})
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
            std::make_pair("typeID", std::to_string(col.second.typeID)),
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
}

std::vector<Table::ColVal> TableMgr::showTables()
{
    if (!curDb.isOk())
        throw NoDBInUseException();
    return sysTables.select({"table"}, {
        std::make_pair("db", std::vector<Table::ConLiteral>({(Table::ConLiteral){Table::EQ, curDb.ok()}})),
    });
}

