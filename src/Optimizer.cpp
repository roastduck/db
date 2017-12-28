#include <unordered_map>
#include "Optimizer.h"

Table::Index Optimizer::optSelectTableList(const TableMgr &mgr, const Table::Index &in, const TableMgr::OuterConsMap &cons)
{
    std::unordered_map<std::string, int> degree;
    for (const auto tb : in)
        degree[tb] = 0; // Income degree. -1 means already used
    std::unordered_map< std::string, std::vector<std::string> > edge; // Adjacency table
    for (const auto &item : cons)
        for (const auto con : item.second)
        {
            if (con.dir != Table::EQ)
                continue;
            bool lIsIdx = mgr.isIndex(item.first.first, {con.col1});
            bool rIsIdx = mgr.isIndex(item.first.second, {con.col2});
            if (!lIsIdx && rIsIdx) // Search left side first
            {
                edge[item.first.first].push_back(item.first.second);
                degree.at(item.first.second)++;
            }
            if (lIsIdx && !rIsIdx) // Search right side first
            {
                edge[item.first.second].push_back(item.first.first);
                degree.at(item.first.first)++;
            }
        }

    Table::Index out;
    out.reserve(in.size());
    for (int i = 0; i < int(in.size()); i++)
    {
        const std::string *next = NULL;
        int curDgr = 0x7fffffff;
        for (const auto &tb : in)
        {
            const int dgr = degree.at(tb);
            if (dgr != -1 && dgr < curDgr)
                next = &tb, curDgr = dgr;
        }
        out.push_back(*next);
        degree.at(*next) = -1;
        if (edge.count(*next))
            for (const auto &tb : edge.at(*next))
                degree.at(tb)--;
    }
    return out;
}

