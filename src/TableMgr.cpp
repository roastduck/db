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
            std::make_pair("length", (Column){ Type::INT, 0, false }),
            std::make_pair("notNull", (Column){ Type::INT, 0, false })
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
      }, None(), {Table::Index({"db", "table"})})
{}

bool TableMgr::nameExists(Table &table, const std::string &col, const std::string &name)
{
    auto result = table.select({}, Table::ConsL({std::make_pair(col, std::vector<Table::ConLiteral>({
        {Table::EQ, name}
    }))}));
    return !result.empty();
}

/************************************/
/* DB Managements                   */
/************************************/

void TableMgr::createDb(const std::string &name)
{
    try
    {
        sysDbs.insert(Table::ColL({std::make_pair("db", name)}));
    } catch (const NotUniqueException &e)
    {
        throw IDAlreadyUsedException("database", name);
    }
}

void TableMgr::use(const std::string &name)
{
    if (!nameExists(sysDbs, "db", name))
        throw NoSuchThingException("database", name);
    db = name;
}

void TableMgr::dropDb(const std::string &name)
{
    if (!nameExists(sysDbs, "db", name))
        throw NoSuchThingException("database", name);
    sysDbs.remove(Table::ConsL({std::make_pair("db", std::vector<Table::ConLiteral>({
        {Table::EQ, name}
    }))}));
    if (db.isOk() && name == db.ok())
        db = None();
}

std::vector<Table::ColVal> TableMgr::showDbs()
{
    return sysDbs.select({"db"}, {});
}
