#include "TablePages.h"

TablePages::TablePages(
    PageCache &_cache, const std::string &_tableName, const TablePages::Cols &_cols,
    const Optional<TablePages::Index> &_primary, const std::vector<TablePages::Index> &_nonClus
)
    : cache(_cache), recCols(_cols),
      dataFile(_tableName + ".data.db"), freeListFile(_tableName + ".freelist.db")
{
    if (_primary.isOk())
    {
        for (const std::string &name : _primary.ok())
        {
            refCols[name] = recCols.at(name);
            priCols[name] = recCols.at(name);
        }
        priCols["$child"] = (Column){Type::INT, 0, true};
    } else
        refCols["$page"] = (Column){Type::INT, 0, true};

    nonClusCols.reserve(_nonClus.size());
    for (const auto &item : _nonClus)
    {
        Cols node;
        for (const std::string &name : item)
            node[name] = recCols.at(name);
        node["$child"] = (Column){Type::INT, 0, true};
        nonClusCols.push_back(std::move(node));
    }
}

TablePages::Cols TablePages::identToCols(short ident)
{
    switch (ident)
    {
    case INVALID:
        assert(false);
    case RECORD:
        return recCols;
    case REF:
        return refCols;
    case PRIMARY:
        return priCols;
    default:
        return nonClusCols.at(ident - NON_CLUSTER);
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

bool TablePages::isFree(int pageID)
{
    const int NUM_PER_LIST = PageMgr::PAGE_SIZE * 8;
    return getFreeListPage(pageID / NUM_PER_LIST).get(pageID % NUM_PER_LIST);
}

ListPage &TablePages::getDataPage(int pageID)
{
    assert(pageID >= 0);
    dataPages.reserve(pageID + 1);
    for (int i = dataPages.size(); i <= pageID; i++)
    {
        std::unique_ptr<ListPage> item(new ListPage(cache, dataFile, i));
        short ident = item->getIdent();
        if (ident != INVALID)
            item->setCols(identToCols(ident));
        dataPages.push_back(std::move(item));
    }
    return *dataPages[pageID];
}

BitmapPage &TablePages::getFreeListPage(int pageID)
{
    assert(pageID >= 0);
    freeListPages.reserve(pageID + 1);
    while (int(freeListPages.size()) <= pageID)
        freeListPages.push_back(std::unique_ptr<BitmapPage>(new BitmapPage(cache, freeListFile, freeListPages.size())));
    return *freeListPages[pageID];
}

int TablePages::newDataPage(short ident, int indexID)
{
    assert(ident > 0);
    int pageID = nextFreeDataPage();
    ListPage &page = getDataPage(pageID);
    if (ident >= NON_CLUSTER && indexID != -1)
        ident = NON_CLUSTER + indexID;
    page.setSize(0);
    page.setNext(-1);
    page.setPrev(-1);
    page.setIdent(ident);
    page.setCols(identToCols(ident));
    return pageID;
}

void TablePages::destroyDataPage(int pageID)
{
    ListPage &page = getDataPage(pageID);
    int nextID = page.getNext();
    int prevID = page.getPrev();
    if (nextID != -1)
        getDataPage(nextID).setPrev(prevID);
    if (prevID != -1)
        getDataPage(prevID).setNext(nextID);
    const int NUM_PER_LIST = PageMgr::PAGE_SIZE * 8;
    getFreeListPage(pageID / NUM_PER_LIST).reset(pageID % NUM_PER_LIST);
}

