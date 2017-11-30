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

