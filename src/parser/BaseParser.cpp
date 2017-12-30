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
    const BaseParser::Tgt &_orderBy, const Tgt &_groupBy, BaseParser::Agg _agg
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

    if (!_orderBy.empty())
        Table::sort(result.begin(), result.end(), getPlainTgt(getFullTgt(_orderBy, tableList), tableList));

    if ((!_agg.empty() || !_groupBy.empty()) && !result.empty())
    {
        getFullAgg(_agg, tableList);
        auto agg = getPlainAgg(_agg);

        auto groupBy = getPlainTgt(getFullTgt(_groupBy, tableList), tableList);
        if (!groupBy.empty())
            Table::stableSort(result.begin(), result.end(), groupBy);

        std::vector<Table::ColVal> newResult;
        std::unordered_map<std::string, int> n;
        Table::ColVal cur;
        for (const auto &col : result[0])
            cur[col.first] = nullptr, n[col.first] = 0;
        for (int i = 0; i < int(result.size()); i++) // Items of result will be destoryed
        {
            bool restart = (i + 1 == int(result.size()));
            for (auto j = groupBy.begin(); !restart && j != groupBy.end(); j++)
                if (!Type::equal(result[i + 1].at(*j), result[i].at(*j)))
                    restart = true;

            for (auto &col : result[i])
            {
                if (col.second != nullptr)
                    n.at(col.first)++;
                if (!agg.count(col.first))
                    cur.at(col.first) = std::move(col.second);
                else
                    Aggregate::add(agg.at(col.first), cur.at(col.first), std::move(col.second));
            }
            // NOTE: Can't use `result[i]` under here

            if (restart)
            {
                for (const auto &item : agg)
                    Aggregate::fin(item.second, cur.at(item.first), n.at(item.first));
                newResult.push_back(std::move(cur)), cur.clear();
                for (const auto &col : result[0])
                    cur[col.first] = nullptr, n[col.first] = 0;
            }
        }
        result = std::move(newResult);
    }

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

void BaseParser::getFullAgg(BaseParser::Agg &agg, const std::vector<std::string> &tableList)
{
    if (agg.count(""))
    {
        if (tableList.size() == 1)
        {
            for (const auto &item : agg.at(""))
                agg[tableList[0]][item.first] = item.second;
            agg.erase("");
        } else
            for (const auto &item : agg.at(""))
                throw IllegalFieldException("", item.first);
    }
}

BaseParser::PlainAgg BaseParser::getPlainAgg(const BaseParser::Agg &agg)
{
    PlainAgg ret;
    for (const auto &item : agg)
        for (const auto &col : item.second)
            ret[item.first + "." + col.first] = col.second;
    return ret;
}

