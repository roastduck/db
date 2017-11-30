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

