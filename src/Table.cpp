#include <cassert>
#include "Table.h"

Table::Table(PageCache &_cache, const std::string &_tableName, const std::unordered_map<std::string, Column> &_cols)
    : cache(_cache), cols(_cols),
      dataFile(_tableName + ".data.db"), freeListFile(_tableName + ".freelist.db")
    {}

Table::Cons Table::genConstraints(const Table::ConsL &literals)
{
    Cons ret;
    for (const auto &pair : literals)
    {
        const std::string &name = pair.first;
        for (const auto &literal : pair.second)
        {
            ConValue value;
            value.dir = literal.dir;
            value.pivot = Type::newFromLiteral(literal.pivot, cols.at(name).typeID, cols.at(name).length);
            ret[name].push_back(std::move(value));
        }
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
    assert(pageID >= 0);
    while (int(dataPages.size()) <= pageID)
        dataPages.push_back(ListPage(cache, dataFile, dataPages.size(), cols));
    // TODO: when adding index, it should be construct according to Ident
    return dataPages[pageID];
}

BitmapPage &Table::getFreeListPage(int pageID)
{
    assert(pageID >= 0);
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

void Table::destroyDataPage(int pageID)
{
    ListPage &page = getDataPage(pageID);
    if (!pageID) // Page 0 is the entry, which can't be deleted
        return;
    int nextID = page.getNext();
    int prevID = page.getPrev();
    if (nextID != -1)
        getDataPage(nextID).setPrev(prevID);
    if (prevID != -1)
        getDataPage(prevID).setNext(nextID);
    const int NUM_PER_LIST = PageMgr::PAGE_SIZE * 8;
    getFreeListPage(pageID / NUM_PER_LIST).reset(pageID % NUM_PER_LIST);
}

bool Table::meetCons(ListPage &page, int rank, const Table::Cons &cons)
{
    for (const auto &pair : cons)
    {
        const std::string &name = pair.first;
        for (const ConValue &rhs : pair.second)
        {
            std::unique_ptr<Type> lhs = page.getValue(rank, name);
            switch (rhs.dir)
            {
            case EQ:
                if (!(*lhs == *rhs.pivot)) return false;
                break;
            case NE:
                if (!(*lhs != *rhs.pivot)) return false;
                break;
            case LT:
                if (!(*lhs < *rhs.pivot)) return false;
                break;
            case LE:
                if (!(*lhs <= *rhs.pivot)) return false;
                break;
            case GT:
                if (!(*lhs > *rhs.pivot)) return false;
                break;
            case GE:
                if (!(*lhs >= *rhs.pivot)) return false;
                break;
            default: assert(false);
            }
        }
    }
    return true;
}

void Table::insert(const Table::ColVal &vals)
{
    if (!getFreeListPage(0).get(0))
        getFreeListPage(0).set(0);
    int pageID = 0;
    while (true)
    {
        ListPage *page = &getDataPage(pageID);
        int oldSize = page->getSize();
        if (oldSize < page->getMaxSize())
        {
            for (const auto &pair : vals)
                page->setValue(oldSize, pair.first, pair.second);
            page->setSize(oldSize + 1);
            break;
        }

        int nextID = page->getNext();
        if (nextID == -1)
        {
            nextID = newDataPage();
            // we cannot use *page because dataPages is a vector and is updated
            getDataPage(pageID).setNext(nextID);
            getDataPage(nextID).setPrev(pageID);
        }
        pageID = nextID;
    }
}

void Table::remove(const Cons &constraints)
{
    // FUTURE: If we keep the removed record in place marked deleted, we can optimize table without primary index
    if (!getFreeListPage(0).get(0))
        return;
    int pageID = 0;
    while (true)
    {
        ListPage *page = &getDataPage(pageID);
        int oldSize = page->getSize(), size = oldSize;
        for (int i = 0, j = 0; i < oldSize; i++)
            if (meetCons(*page, i, constraints))
                size--;
            else
                page->copy(i, j++);
        page->setSize(size);
        if (!size)
            destroyDataPage(pageID);
        int nextID = page->getNext();
        if (!~nextID) return;
        pageID = nextID;
    }
}

std::vector<Table::ColVal> Table::select(const std::vector<std::string> &targets, const Cons &constraints)
{
    if (!getFreeListPage(0).get(0))
        return {};
    std::vector<Table::ColVal> ret;
    int pageID = 0;
    while (true)
    {
        ListPage *page = &getDataPage(pageID);
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
        pageID = nextID;
    }
}

