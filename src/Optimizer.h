#ifndef OPTIMIZER_H_
#define OPTIMIZER_H_

#include "Table.h"
#include "TableMgr.h"

class Optimizer
{
public:
    /** Optimize the order of tables when selecting from multiple tables
     *  Algorithm:
     *  If there's a constraint `t1.a = t2.b`, where `t1.a` is a index while `t2.b` is not,
     *  we want to search `t2` first. We consider this problem as a graph, from which we want
     *  to delete as few edges as possible, to make the graph to be a DAG, and calculate its
     *  topologic order
     */
    static Table::Index optSelectTableList(const TableMgr &mgr, const Table::Index &in, const TableMgr::OuterConsMap &cons);
};

#endif // OPTIMIZER_H_
