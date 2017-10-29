#include <cassert>
#include "Table.h"

Table::Table(PageCache &_cache, const std::string &_tableName, const std::unordered_map<std::string, Column> &_cols)
    : cache(_cache), cols(_cols),
      dataFile(_tableName + ".data.db"), freeListFile(_tableName + ".freelist.db")
    {}

Table::Cons Table::genConstraints(const std::unordered_map<std::string, Table::ConLiteral> &literals)
{
    Cons ret;
    for (const auto &pair : literals)
    {
        const std::string &name = pair.first;
        const ConLiteral &literal = pair.second;
        ConValue value;
        value.dir = literal.dir;
        value.pivot = Type::newFromLiteral(literal.pivot, cols.at(name).typeID, cols.at(name).length);
        ret[name] = std::move(value);
    }
    return ret;
}

Table::ColVal Table::genVals(const std::unordered_map<std::string, std::string> &literals)
{
    ColVal ret;
    for (const auto &pair : literals)
        ret[pair.first] = Type::newFromLiteral(pair.second, cols.at(pair.first).typeID, cols.at(pair.first).length);
    return ret;
}

ListPage &Table::getDataPage(int pageID)
{
    while (int(dataPages.size()) <= pageID)
        dataPages.push_back(ListPage(cache, dataFile, dataPages.size(), cols));
    // TODO: when adding index, it should be construct according to Ident
    return dataPages[pageID];
}

BitmapPage &Table::getFreeListPage(int pageID)
{
    while (int(freeListPages.size()) <= pageID)
        freeListPages.push_back(BitmapPage(cache, freeListFile, freeListPages.size()));
    return freeListPages[pageID];
}

int Table::newDataPage()
{
    for (int i = 0;; i++)
    {
        BitmapPage &page = getFreeListPage(i);
        int offset = page.first0();
        if (offset != -1)
        {
            assert(!page.get(offset));
            page.set(offset);
            return i * PageMgr::PAGE_SIZE * 8 + offset;
        }
    }
}

bool Table::meetCons(ListPage &page, int rank, const Table::Cons &cons)
{
    for (const auto &pair : cons)
    {
        const std::string &name = pair.first;
        const ConValue &rhs = pair.second;
        std::unique_ptr<Type> lhs = page.getValue(rank, name);
        switch (rhs.dir)
        {
        case EQ:
            if (!(lhs == rhs.pivot)) return false;
        case NE:
            if (!(lhs != rhs.pivot)) return false;
        case LT:
            if (!(lhs < rhs.pivot)) return false;
        case LE:
            if (!(lhs <= rhs.pivot)) return false;
        case GT:
            if (!(lhs > rhs.pivot)) return false;
        case GE:
            if (!(lhs >= rhs.pivot)) return false;
        default: assert(false);
        }
    }
    return true;
}

void Table::insert(const Table::ColVal &vals)
{
    int pageID = 0;
    ListPage *page = &getDataPage(pageID);
    while (true)
    {
        int oldSize = page->getSize();
        if (oldSize < page->getMaxSize())
        {
            for (const auto &pair : vals)
                page->setValue(oldSize, pair.first, pair.second);
            page->setSize(oldSize + 1);
            break;
        }

        int nextID = page->getNext();
        ListPage *next;
        if (nextID != -1)
            next = &getDataPage(nextID);
        else
        {
            nextID = newDataPage();
            next = &getDataPage(nextID);
            page->setNext(nextID);
            next->setPrev(pageID);
        }
        pageID = nextID, page = next;
    }
}

void Table::remove(const Cons &constraints)
{
    ListPage *page = &getDataPage(0);
    while (true)
    {
        int oldSize = page->getSize(), size = oldSize;
        for (int i = 0, j = 0; i < oldSize; i++)
            if (meetCons(*page, i, constraints))
                size--;
            else
                page->copy(i, j++);
        page->setSize(size);
        int nextID = page->getNext();
        if (!~nextID) return;
        page = &getDataPage(nextID);
    }
}

std::vector<Table::ColVal> Table::select(const std::vector<std::string> &targets, const Cons &constraints)
{
    std::vector<Table::ColVal> ret;
    ListPage *page = &getDataPage(0);
    while (true)
    {
        int size = page->getSize();
        for (int i = 0; i < size; i++)
            if (meetCons(*page, i, constraints))
            {
                ColVal item;
                for (const std::string &name : targets)
                    item[name] = page->getValue(i, name);
                ret.push_back(std::move(item));
            }
        int nextID = page->getNext();
        if (!~nextID) return ret;
        page = &getDataPage(nextID);
    }
}

