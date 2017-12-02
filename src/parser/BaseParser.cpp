#include "BaseParser.h"

void BaseParser::showDbs()
{
    output->addResult(tableMgr->showDbs());
}

void BaseParser::createDb(const std::string &name)
{
    tableMgr->createDb(name);
    output->addInfo("Created database " + name);
}

void BaseParser::dropDb(const std::string &name)
{
    tableMgr->dropDb(name);
    output->addInfo("Dropped database " + name);
}

void BaseParser::use(const std::string &name)
{
    tableMgr->use(name);
    output->addInfo("Database changed to " + name);
}

void BaseParser::showTables()
{
    output->addResult(tableMgr->showTables());
}

void BaseParser::createTable(
    const std::string &name, const BaseParser::Cols &cols, const BaseParser::PriIdx &priIdx, const BaseParser::Fors &fors
)
{
    tableMgr->createTable(name, cols, priIdx, {}, fors);
    output->addInfo("Created table " + name);
}

void BaseParser::dropTable(const std::string &name)
{
    tableMgr->dropTable(name);
    output->addInfo("Dropped table " + name);
}

void BaseParser::desc(const std::string &name)
{
    output->addResult(tableMgr->desc(name), {TableMgr::FIELD, TableMgr::TYPE, TableMgr::NOT_NULL, TableMgr::IS_PRI});
}

void BaseParser::insert(const std::string &tbName, const VLists &valueLists)
{
    tableMgr->insert(tbName, valueLists);
    output->addInfo("Inserted " + std::to_string(valueLists.size()) + " record(s) into table " + tbName);
}

void BaseParser::remove(const std::string &tbName, const ICM &icm, const OCM &ocm)
{
    tableMgr->remove(tbName, getTableIC(tbName, icm), getTableOC(tbName, ocm));
    output->addInfo("Deleted from table " + tbName);
}

Table::ConsL BaseParser::getTableIC(const std::string &tbName, const ICM &icm)
{
    for (const auto &i : icm)
        if (i.first != tbName)
            for (const auto &j : i.second)
                throw IllegalFieldException(i.first, j.first);
    const auto ret = icm.find(tbName);
    return ret == icm.end() ? Table::ConsL() : ret->second;
}

Table::OuterCons BaseParser::getTableOC(const std::string &tbName, const OCM &ocm)
{
    for (const auto &i : ocm)
    {
        if (i.first.first != tbName)
            for (const auto &j : i.second)
                throw IllegalFieldException(i.first.first, j.col1);
        if (i.first.second != tbName)
            for (const auto &j : i.second)
                throw IllegalFieldException(i.first.second, j.col2);
    }
    const auto ret = ocm.find(std::make_pair(tbName, tbName));
    return ret == ocm.end() ? Table::OuterCons() : ret->second;
}

