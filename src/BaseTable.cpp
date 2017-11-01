#include <cassert>
#include "BaseTable.h"
#include "type/IntType.h"
#include "exception/NotUniqueException.h"

BaseTable::BaseTable
(
    PageCache &_cache, const std::string &_tableName, const Cols &_cols,
    const Optional<Index> &_primary, const std::vector<Index> &_nonClus
)
    : TablePages(_cache, _tableName, _cols, _primary, _nonClus),
      primary(_primary), nonClus(_nonClus)
{
    // reserve 0 for primary entrance, 1 ~ nonClus.size() for non-clustered index entrance
    for (int i = 0; i <= int(nonClus.size()); i++)
        if (!isFree(i))
        {
            int pageID = newDataPage(!i ? RECORD : NON_CLUSTER); // RECORD is the leaf node of PRIMARY
            assert(pageID == i);
        }
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
                if (lhs == nullptr || !(*lhs == *rhs.pivot)) return false;
                break;
            case NE:
                if (lhs == nullptr || !(*lhs != *rhs.pivot)) return false;
                break;
            case LT:
                if (lhs == nullptr || !(*lhs < *rhs.pivot)) return false;
                break;
            case LE:
                if (lhs == nullptr || !(*lhs <= *rhs.pivot)) return false;
                break;
            case GT:
                if (lhs == nullptr || !(*lhs > *rhs.pivot)) return false;
                break;
            case GE:
                if (lhs == nullptr || !(*lhs >= *rhs.pivot)) return false;
                break;
            default: assert(false);
            }
        }
    }
    return true;
}

bool BaseTable::less(const BaseTable::ColVal &lhs, const BaseTable::ColVal &rhs, const BaseTable::Index &order)
{
    for (const std::string &name : order)
    {
        const std::unique_ptr<Type> &l = lhs.at(name), &r = rhs.at(name);
        if (l == nullptr && r == nullptr)
            continue;
        if (l == nullptr)
            return true;
        if (r == nullptr)
            return false;
        if (*l < *r)
            return true;
        if (*l > *r)
            return false;
    }
    return false;
}

bool BaseTable::equal(const BaseTable::ColVal &lhs, const BaseTable::ColVal &rhs, const BaseTable::Index &order)
{
    for (const std::string &name : order)
    {
        const std::unique_ptr<Type> &l = lhs.at(name), &r = rhs.at(name);
        if (l == nullptr && r == nullptr)
            continue;
        if (l == nullptr)
            return false;
        if (r == nullptr)
            return false;
        if (*l != *r)
            return false;
    }
    return true;
}

int BaseTable::insertAndSplit(int pageID, const BaseTable::ColVal &vals, int off)
{
    int ret = -1;
    ListPage *pageL = &getDataPage(pageID), *pageIns = pageL;
    if (pageL->getSize() == pageL->getMaxSize())
    {
        ListPage *pageR = &getDataPage(ret = newDataPage(pageL->getIdent()));
        int mid = pageL->getSize() / 2;
        for (int i = mid, j = 1; i < pageL->getSize(); i++, j++)
            ListPage::copy(pageL, i, pageR, j);
        pageR->setSize(pageL->getSize() - mid);
        pageL->setSize(mid);
        if (off >= mid)
            off -= mid, pageIns = pageR;
    }
    for (int i = off; i < pageIns->getSize(); i++)
        pageIns->copy(i, i + 1);
    pageIns->setSize(pageIns->getSize() + 1);
    pageIns->setValues(off, vals);
    return ret;
}

int BaseTable::insertRecur(int pageID, const BaseTable::ColVal &vals, const BaseTable::Index &index)
{
    ListPage *page;
    int size;
    page = &getDataPage(pageID), size = page->getSize();
    int offset = 0;
    while (offset < size && less(page->getValues(offset, index), vals, index))
        offset++;
    // `offset` is the first to >= `vals`

    if (page->getIdent() == RECORD) // leaf node of primary index
    {
        if (equal(page->getValues(offset, index), vals, index))
            throw NotUniqueException(index);
        return insertAndSplit(pageID, vals, offset);
    }
    int childID = dynamic_cast<IntType*>(page->getValue(offset, "$child").get())->getVal();
    page = NULL;
    if (getDataPage(childID).getIdent() == REF) // leaf node of non-clustered index
    {
        ColVal newRef;
        if (primary.isOk())
            for (const std::string &name : primary.ok())
                newRef[name] = Type::newFromCopy(vals.at(name));
        else
            newRef["$page"] = Type::newFromCopy(vals.at("$page"));
        if (equal(page->getValues(offset, index), vals, index))
        {
            // insert into the ref page
            insertLinear(childID, newRef);
            return -1;
        } else
        {
            // generate a new ref page and insert as a child
            int childID = newDataPage(REF);
            insertLinear(childID, newRef);
            ColVal newItem;
            for (const std::string &name : index)
                newItem[name] = Type::newFromCopy(vals.at(name));
            newItem["$child"] = Type::newType(Type::INT);
            dynamic_cast<IntType*>(newItem["$child"].get())->setVal(childID);
            return insertAndSplit(pageID, newItem, offset);
        }
    }

    // recurse
    if (offset == size)
    {
        offset--;
        assert(offset >= 0);
        childID = dynamic_cast<IntType*>(page->getValue(offset, "$child").get())->getVal();
    }
    int newChildID = insertRecur(childID, vals, index);
    if (!~newChildID)
        return -1;
    ColVal newItem = getDataPage(newChildID).getValues(0, index);
    newItem["$child"] = Type::newType(Type::INT);
    dynamic_cast<IntType*>(newItem["$child"].get())->setVal(newChildID);
    return insertAndSplit(pageID, newItem, offset + 1);
}

void BaseTable::insertLinear(int pageID, const BaseTable::ColVal &vals)
{
    while (true)
    {
        ListPage &page = getDataPage(pageID);
        int oldSize = page.getSize();
        if (oldSize < page.getMaxSize())
        {
            for (const auto &pair : vals)
                page.setValue(oldSize, pair.first, pair.second);
            page.setSize(oldSize + 1);
            break;
        }

        int nextID = page.getNext();
        if (nextID == -1)
        {
            nextID = newDataPage(RECORD);
            // we cannot use `page` because dataPages is a vector and is updated
            getDataPage(pageID).setNext(nextID);
            getDataPage(nextID).setPrev(pageID);
        }
        pageID = nextID;
    }
}

void BaseTable::insert(const BaseTable::ColVal &vals)
{
    if (primary.isOk())
    {
        int newChildRID = insertRecur(0, vals, primary.ok());

        // root should still be page 0
        int newChildLID = newDataPage(PRIMARY);
        ListPage *newChildL = &getDataPage(newChildLID); // this should before `newRoot`
        ListPage *newRoot = &getDataPage(0);
        for (int i = 0; i < newRoot->getSize(); i++)
            ListPage::copy(newRoot, i, newChildL, i);
        newRoot = newChildL = NULL;

        ColVal newItemL = getDataPage(newChildLID).getValues(0, primary.ok());
        newItemL["$child"] = Type::newType(Type::INT);
        dynamic_cast<IntType*>(newItemL["$child"].get())->setVal(newChildLID);
        getDataPage(0).setValues(0, newItemL);

        ColVal newItemR = getDataPage(newChildRID).getValues(0, primary.ok());
        newItemR["$child"] = Type::newType(Type::INT);
        dynamic_cast<IntType*>(newItemR["$child"].get())->setVal(newChildRID);
        getDataPage(0).setValues(1, newItemR);

        getDataPage(0).setSize(2);
    } else
        insertLinear(0, vals);
    // TODO : insert into non-clustered indexes
}

void BaseTable::remove(const ConsVal &constraints)
{
    // FUTURE: If we keep the removed record in place marked deleted, we can optimize table without primary index
    int pageID = 0;
    while (true)
    {
        ListPage &page = getDataPage(pageID);
        int oldSize = page.getSize(), size = oldSize;
        for (int i = 0, j = 0; i < oldSize; i++)
            if (meetCons(page, i, constraints))
                size--;
            else
                page.copy(i, j++);
        page.setSize(size);
        if (!size)
            destroyDataPage(pageID);
        int nextID = page.getNext();
        if (!~nextID) return;
        pageID = nextID;
    }
}

std::vector<BaseTable::ColVal> BaseTable::select(const BaseTable::Index &targets, const ConsVal &constraints)
{
    std::vector<BaseTable::ColVal> ret;
    int pageID = 0;
    while (true)
    {
        ListPage &page = getDataPage(pageID);
        int size = page.getSize();
        for (int i = 0; i < size; i++)
            if (meetCons(page, i, constraints))
                ret.push_back(page.getValues(i, targets));
        int nextID = page.getNext();
        if (!~nextID) return ret;
        pageID = nextID;
    }
}

