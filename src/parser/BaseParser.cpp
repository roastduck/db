#include <cassert>
#include "../utils.h"
#include "BaseParser.h"
#include "../type/CharType.h"

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
    const std::string &name, const BaseParser::Cols &cols, const BaseParser::PriIdx &priIdx,
    const BaseParser::Fors &fors, const BaseParser::Chk &chk
)
{
    tableMgr->createTable(name, cols, priIdx, {}, fors, chk);
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

void BaseParser::remove(const std::string &tbName, const BaseParser::ICM &icm, const BaseParser::OCM &ocm)
{
    tableMgr->remove(tbName, getTableIC(tbName, icm), getTableOC(tbName, ocm));
    output->addInfo("Deleted from table " + tbName);
}

void BaseParser::update(
    const std::string &tbName, const Table::ColL &setClause, const BaseParser::ICM &icm, const BaseParser::OCM &ocm
)
{
    tableMgr->update(tbName, setClause, getTableIC(tbName, icm), getTableOC(tbName, ocm));
    output->addInfo("Updated table " + tbName);
}

void BaseParser::select(
    const Optional<BaseParser::Tgt> &_targets, const std::vector<std::string> &tableList,
    const BaseParser::ICM &icm, const BaseParser::OCM &ocm,
    const Tgt &orderBy
)
{
    for (const auto &i : icm)
        if (i.first.empty())
            for (const auto &j : i.second)
                throw IllegalFieldException(i.first, j.first);
    for (const auto &i : ocm)
    {
        if (i.first.first.empty())
            for (const auto &j : i.second)
                throw IllegalFieldException(i.first.first, j.col1);
        if (i.first.second.empty())
            for (const auto &j : i.second)
                throw IllegalFieldException(i.first.second, j.col2);
    }

    assert(!tableList.empty());
    Tgt targets;
    if (_targets.isOk())
        targets = getFullTgt(_targets.ok(), tableList);
    else
        for (const auto &tb : tableList)
            for (const auto &col : tableMgr->desc(tb))
                targets[tb].push_back(dynamic_cast<CharType*>(col.at(TableMgr::FIELD).get())->getVal());

    auto result = tableMgr->select(targets, tableList, icm, ocm);
    if (!orderBy.empty())
        Table::sort(result.begin(), result.end(), getPlainTgt(getFullTgt(orderBy, tableList), tableList));

    output->addResult(result, getPlainTgt(targets, tableList));
}

void BaseParser::createIndex(const std::string &tbName, const Table::Index &index)
{
    tableMgr->createIndex(tbName, index);
    output->addInfo("Created index (" + commaJoin(index) + ") for table " + tbName);
}

void BaseParser::dropIndex(const std::string &tbName, const Table::Index &index)
{
    tableMgr->dropIndex(tbName, index);
    output->addInfo("Deleted index (" + commaJoin(index) + ") from table " + tbName);
}

Table::ConsL BaseParser::getTableIC(const std::string &tbName, const BaseParser::ICM &icm)
{
    for (const auto &i : icm)
        if (i.first != tbName)
            for (const auto &j : i.second)
                throw IllegalFieldException(i.first, j.first);
    const auto ret = icm.find(tbName);
    return ret == icm.end() ? Table::ConsL() : ret->second;
}

Table::OuterCons BaseParser::getTableOC(const std::string &tbName, const BaseParser::OCM &ocm)
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

BaseParser::Tgt BaseParser::getFullTgt(BaseParser::Tgt targets, const std::vector<std::string> &tableList)
{
    if (targets.count(""))
    {
        if (tableList.size() == 1)
        {
            for (const auto &tgt : targets.at(""))
                targets[tableList[0]].push_back(tgt);
            targets.erase("");
        } else
            for (const auto &tgt : targets.at(""))
                throw IllegalFieldException("", tgt);
    }
    return targets;
}

Table::Index BaseParser::getPlainTgt(const BaseParser::Tgt &targets, const std::vector<std::string> &tableList)
{
    Table::Index ret;
    for (const auto &tb : tableList) // Can be improved here
        if (targets.count(tb))
            for (const auto &col : targets.at(tb))
                ret.push_back(tb + "." + col);
    return ret;
}

