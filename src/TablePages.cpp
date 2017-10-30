#include "TablePages.h"

TablePages::TablePages(
    PageCache &_cache, const std::string &_tableName, const TablePages::Cols &_cols,
    const Optional<TablePages::Cols> &_primary, const std::vector<TablePages::Cols> &_nonClus
)
    : cache(_cache), recCols(_cols),
      dataFile(_tableName + ".data.db"), freeListFile(_tableName + ".freelist.db")
{
    if (_primary.isOk())
    {
        priCols = _primary.ok();
        priCols["$child"] = (Column){Type::INT, 0, true, {}};
    }
    nonClusCols.reserve(_nonClus.size());
    for (const auto &item : _nonClus)
    {
        Cols node = item, leaf = item;
        node["$child"] = (Column){Type::INT, 0, true, {}};
        if (!_primary.isOk())
            leaf["$child"] = (Column){Type::INT, 0, true, {}};
        else
            for (const auto &pair : _primary.ok())
                leaf["$" + pair.first] = pair.second;
        nonClusCols.push_back(std::make_pair(std::move(node), std::move(leaf)));
    }
}

TablePages::Cols TablePages::identToCols(short ident)
{
    int rank, isLeaf;
    switch (ident)
    {
    case INVALID:
        assert(false);
        break;
    case RECORD:
        return recCols;
    case PRIMARY:
        return priCols;
    default:
        rank = (ident - PRIMARY - 1) / 2;
        isLeaf = (ident - PRIMARY - 1) % 2;
        if (isLeaf)
            return nonClusCols.at(rank).second;
        else
            return nonClusCols.at(rank).first;
    }
}

int TablePages::nextFreeDataPage()
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

ListPage &TablePages::getDataPage(int pageID)
{
    assert(pageID >= 0);
    dataPages.reserve(pageID + 1);
    for (int i = dataPages.size(); i <= pageID; i++)
    {
        ListPage item(ListPage(cache, dataFile, i));
        short ident = item.getIdent();
        if (ident != INVALID)
            item.setCols(identToCols(ident));
        dataPages.push_back(std::move(item));
    }
    return dataPages[pageID];
}

BitmapPage &TablePages::getFreeListPage(int pageID)
{
    assert(pageID >= 0);
    freeListPages.reserve(pageID + 1);
    while (int(freeListPages.size()) <= pageID)
        freeListPages.push_back(BitmapPage(cache, freeListFile, freeListPages.size()));
    return freeListPages[pageID];
}

int TablePages::newRecordDataPage()
{
    int pageID = nextFreeDataPage();
    ListPage &page = getDataPage(pageID);
    page.setIdent(RECORD);
    page.setCols(identToCols(RECORD));
    return pageID;
}

int TablePages::newPrimaryDataPage()
{
    int pageID = nextFreeDataPage();
    ListPage &page = getDataPage(pageID);
    page.setIdent(PRIMARY);
    page.setCols(identToCols(RECORD));
    return pageID;
}

int TablePages::newNonClusNodeDataPage(int indexID)
{
    int pageID = nextFreeDataPage();
    ListPage &page = getDataPage(pageID);
    page.setIdent(PRIMARY + 1 + indexID * 2);
    page.setCols(identToCols(RECORD));
    return pageID;
}

int TablePages::newNonClusLeafDataPage(int indexID)
{
    int pageID = nextFreeDataPage();
    ListPage &page = getDataPage(pageID);
    page.setIdent(PRIMARY + 1 + indexID * 2 + 1);
    page.setCols(identToCols(RECORD));
    return pageID;
}

void TablePages::destroyDataPage(int pageID)
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

