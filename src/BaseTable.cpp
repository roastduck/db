#include <cassert>
#include "BaseTable.h"

BaseTable::BaseTable(
    PageCache &_cache, const std::string &_tableName, const BaseTable::Cols &_cols,
    const Optional<BaseTable::Cols> &_primary, const std::vector<BaseTable::Cols> &_nonClus
)
    : cols(_cols), cache(_cache), primary(_primary), nonClus(_nonClus),
      dataFile(_tableName + ".data.db"), freeListFile(_tableName + ".freelist.db")
    {}

BaseTable::Cols BaseTable::identToCols(short ident)
{
    assert(ident > 0);
    Cols ret;
    if (ident == RECORD)
        ret = cols;
    else if (ident == PRIMARY)
        ret = primary.ok(), ret["$child"] = (Column){Type::INT, 0, true, {}};
    else
    {
        int rank = (ident - PRIMARY - 1) / 2;
        int isLeaf = (ident - PRIMARY - 1) % 2;
        ret = nonClus.at(rank);
        if (!isLeaf || !primary.isOk())
            ret["$child"] = (Column){Type::INT, 0, true, {}};
        else
            for (const auto &pair : primary.ok())
                ret["$" + pair.first] = pair.second;
    }
    return ret;
}

ListPage &BaseTable::getDataPage(int pageID)
{
    assert(pageID >= 0);
    dataPages.reserve(pageID + 1);
    for (int i = dataPages.size(); i <= pageID; i++)
    {
        ListPage item(ListPage(cache, dataFile, i));
        if (item.getIdent() != INVALID)
            item.setCols(identToCols(item.getIdent()));
        dataPages.push_back(std::move(item));
    }
    return dataPages[pageID];
}

BitmapPage &BaseTable::getFreeListPage(int pageID)
{
    assert(pageID >= 0);
    freeListPages.reserve(pageID + 1);
    while (int(freeListPages.size()) <= pageID)
        freeListPages.push_back(BitmapPage(cache, freeListFile, freeListPages.size()));
    return freeListPages[pageID];
}

int BaseTable::nextFreeDataPage()
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

int BaseTable::newRecordDataPage()
{
    int pageID = nextFreeDataPage();
    ListPage &page = getDataPage(pageID);
    page.setIdent(RECORD);
    page.setCols(identToCols(RECORD));
    return pageID;
}

int BaseTable::newPrimaryDataPage()
{
    int pageID = nextFreeDataPage();
    ListPage &page = getDataPage(pageID);
    page.setIdent(PRIMARY);
    page.setCols(identToCols(RECORD));
    return pageID;
}

int BaseTable::newNonClusNodeDataPage(int indexID)
{
    int pageID = nextFreeDataPage();
    ListPage &page = getDataPage(pageID);
    page.setIdent(PRIMARY + 1 + indexID * 2);
    page.setCols(identToCols(RECORD));
    return pageID;
}

int BaseTable::newNonClusLeafDataPage(int indexID)
{
    int pageID = nextFreeDataPage();
    ListPage &page = getDataPage(pageID);
    page.setIdent(PRIMARY + 1 + indexID * 2 + 1);
    page.setCols(identToCols(RECORD));
    return pageID;
}

void BaseTable::destroyDataPage(int pageID)
{
    ListPage &page = getDataPage(pageID);
    if (!pageID) // Page 0 is the entrance, which can't be deleted
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

bool BaseTable::meetCons(ListPage &page, int rank, const BaseTable::ConsVal &cons)
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

void BaseTable::insert(const BaseTable::ColVal &vals)
{
    if (!getFreeListPage(0).get(0))
        newRecordDataPage();
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
            nextID = newRecordDataPage();
            // we cannot use *page because dataPages is a vector and is updated
            getDataPage(pageID).setNext(nextID);
            getDataPage(nextID).setPrev(pageID);
        }
        pageID = nextID;
    }
}

void BaseTable::remove(const ConsVal &constraints)
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

std::vector<BaseTable::ColVal> BaseTable::select(const std::vector<std::string> &targets, const ConsVal &constraints)
{
    if (!getFreeListPage(0).get(0))
        return {};
    std::vector<BaseTable::ColVal> ret;
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

