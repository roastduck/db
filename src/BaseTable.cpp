#include <cassert>
#include <set>
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

void BaseTable::updNode(int pageID, int offset, const BaseTable::Index &index)
{
    ListPage &page = getDataPage(pageID);
    int childID = dynamic_cast<IntType*>(page.getValue(offset, "$child").get())->getVal();
    page.setValues(offset, getDataPage(childID).getValues(0, index));
}

int BaseTable::insertAndSplit(int pageID, const BaseTable::ColVal &vals, int off)
{
    int ret = -1, insID = pageID;
    ListPage &pageL = getDataPage(pageID);
    if (pageL.getSize() == pageL.getMaxSize())
    {
        ListPage &pageR = getDataPage(ret = newDataPage(pageL.getIdent()));
        int mid = pageL.getSize() / 2;
        for (int i = mid, j = 0; i < pageL.getSize(); i++, j++)
            ListPage::copy(&pageL, i, &pageR, j);
        pageR.setSize(pageL.getSize() - mid);
        pageL.setSize(mid);

        int nextID = pageL.getNext();
        pageL.setNext(ret);
        pageR.setPrev(pageID);
        pageR.setNext(nextID);
        if (nextID != -1)
            getDataPage(nextID).setPrev(ret);

        if (off >= mid)
            off -= mid, insID = ret;
    }
    ListPage &pageIns = getDataPage(insID);
    for (int i = pageIns.getSize() - 1; i >= off; i--)
        pageIns.copy(i, i + 1);
    pageIns.setSize(pageIns.getSize() + 1);
    pageIns.setValues(off, vals);
    return ret;
}

int BaseTable::insertRecur(int pageID, const BaseTable::ColVal &vals, const BaseTable::Index &index)
{
    ListPage &page = getDataPage(pageID);
    int size = page.getSize();
    int offset = -1;
    while (offset + 1 < size && !less(vals, page.getValues(offset + 1, index), index))
        offset++;
    // `offset` is the last that <= `vals`

    if (page.getIdent() == RECORD) // leaf node of primary index
    {
        if (offset >= 0 && equal(page.getValues(offset, index), vals, index))
            throw NotUniqueException(index);
        return insertAndSplit(pageID, vals, offset + 1);
    }

    assert(size > 0);
    short childIdent = getDataPage(dynamic_cast<IntType*>(page.getValue(0, "$child").get())->getVal()).getIdent();
    if (childIdent == REF) // leaf node of non-clustered index
    {
        ColVal newRef;
        if (primary.isOk())
            for (const std::string &name : primary.ok())
                newRef[name] = Type::newFromCopy(vals.at(name));
        else
            newRef["$page"] = Type::newFromCopy(vals.at("$page"));
        if (offset >= 0 && equal(page.getValues(offset, index), vals, index))
        {
            // insert into the ref page
            int childID = dynamic_cast<IntType*>(page.getValue(offset, "$child").get())->getVal();
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
            return insertAndSplit(pageID, newItem, offset + 1);
        }
    }

    // recurse
    if (offset < 0) offset = 0;
    int childID = dynamic_cast<IntType*>(page.getValue(offset, "$child").get())->getVal();
    if (!offset)
        page.setValues(0, getDataPage(childID).getValues(0, index));
    int newChildID = insertRecur(childID, vals, index);
    if (!~newChildID)
        return -1;
    ColVal newItem = getDataPage(newChildID).getValues(0, index);
    newItem["$child"] = Type::newType(Type::INT);
    dynamic_cast<IntType*>(newItem["$child"].get())->setVal(newChildID);
    return insertAndSplit(pageID, newItem, offset + 1);
}

int BaseTable::insertLinear(int pageID, const BaseTable::ColVal &vals)
{
    while (true)
    {
        ListPage &page = getDataPage(pageID);
        int oldSize = page.getSize();
        if (oldSize < page.getMaxSize())
        {
            page.setValues(oldSize, vals);
            page.setSize(oldSize + 1);
            return pageID;
        }

        int nextID = page.getNext();
        if (nextID == -1)
        {
            ListPage &next = getDataPage(nextID = newDataPage(RECORD));
            int nextNextID = page.getNext();
            page.setNext(nextID);
            next.setPrev(pageID);
            next.setNext(nextNextID);
            if (nextNextID != -1)
                getDataPage(nextNextID).setPrev(nextID);
        }
        pageID = nextID;
    }
}

Optional<int> BaseTable::removeAndMerge(int pageID, int off)
{
    ListPage &page = getDataPage(pageID);
    page.setSize(page.getSize() - 1);
    for (int i = off; i < page.getSize(); i++)
        page.copy(i + 1, i);
    const int thres = page.getMaxSize() / 2 + bool(page.getMaxSize() % 2);
    if (page.getSize() >= thres) return None();

    ListPage *prev = !~page.getPrev() ? NULL : &getDataPage(page.getPrev());
    ListPage *next = !~page.getNext() ? NULL : &getDataPage(page.getNext());
    if (prev && prev->getSize() > thres)
    {
        page.setSize(page.getSize() + 1);
        prev->setSize(prev->getSize() - 1);
        for (int i = page.getSize() - 1; i > 0; i--)
            page.copy(i - 1, i);
        ListPage::copy(prev, prev->getSize(), &page, 0);
        return None();
    }
    if (next && next->getSize() > thres)
    {
        page.setSize(page.getSize() + 1);
        next->setSize(next->getSize() - 1);
        ListPage::copy(next, 0, &page, page.getSize() - 1);
        for (int i = 0; i < next->getSize(); i++)
            next->copy(i + 1, i);
        return None();
    }
    if (prev)
    {
        for (int i = 0, j = prev->getSize(); i < page.getSize(); i++, j++)
            ListPage::copy(&page, i, prev, j);
        prev->setSize(prev->getSize() + page.getSize());
        destroyDataPage(pageID);
        return 0;
    }
    if (next)
    {
        for (int i = 0, j = page.getSize(); i < next->getSize(); i++, j++)
            ListPage::copy(next, i, &page, j);
        page.setSize(page.getSize() + next->getSize());
        destroyDataPage(page.getNext());
        return 1;
    }
    // This must be root
    return None();
}

Optional<int> BaseTable::removeRecur(int pageID, const BaseTable::ColVal &vals, const BaseTable::Index &index)
{
    ListPage &page = getDataPage(pageID);
    int size = page.getSize();
    int offset = -1;
    while (offset + 1 < size && !less(vals, page.getValues(offset + 1, index), index))
        offset++;
    // `offset` is the last that <= `vals`
    if (!~offset) return None();

    if (page.getIdent() == RECORD)
    {
        if (equal(page.getValues(offset, index), vals, index))
            return removeAndMerge(pageID, offset);
        return None();
    }
    int childID = dynamic_cast<IntType*>(page.getValue(offset, "$child").get())->getVal();
    ListPage &child = getDataPage(childID);
    if (child.getIdent() == REF)
    {
        if (equal(page.getValues(offset, index), vals, index))
        {
            std::vector<ConValue> v;
            v.push_back({(ConValue){EQ, Type::newFromCopy(vals.at("$page"))}});
            ConsVal c;
            c["$page"] = std::move(v); // move sementic is not supported in initializer_list
            if (removeLinear(childID, std::move(c), true))
                return removeAndMerge(pageID, offset);
        }
        return None();
    }

    // recurse
    auto needRemove = removeRecur(childID, vals, index);
    if (needRemove.isOk())
        return removeAndMerge(pageID, offset + needRemove.ok());
    updNode(pageID, offset, index); // Rotation will destory original data
    if (offset + 1 < page.getSize())
        updNode(pageID, offset + 1, index);
    return None();
}

bool BaseTable::removeLinear(int pageID, const ConsVal &constraints, bool onlyOne)
{
    const int startID = pageID;
    bool removedOne = false, empty = false;
    while (true)
    {
        ListPage &page = getDataPage(pageID);
        int oldSize = page.getSize(), size = oldSize;
        for (int i = 0, j = 0; i < oldSize; i++)
            if (meetCons(page, i, constraints) && (!removedOne || !onlyOne))
                size--, removedOne = true;
            else
                page.copy(i, j++);
        page.setSize(size);
        int nextID = page.getNext();
        if (!size)
        {
            if (pageID == startID)
            {
                if (!~nextID)
                    empty = true;
                else
                {
                    ListPage &next = getDataPage(nextID);
                    page.setSize(next.getSize());
                    page.setNext(next.getNext());
                    page.setPrev(-1);
                    for (int i = 0; i < page.getSize(); i++)
                        ListPage::copy(&next, i, &page, i);
                    std::swap(pageID, nextID);
                }
            }
            destroyDataPage(pageID);
        }
        if (!~nextID) return empty;
        pageID = nextID;
    }
}

BaseTable::Pos BaseTable::findFirst(int pageID, const BaseTable::ColVal &vals, const BaseTable::Index &index, bool equal)
{
    Pos ret;
    while (true)
    {
        ListPage &page = getDataPage(pageID);
        int size = page.getSize();
        int offset = 0;
        if (equal)
            while (offset + 1 < size && less(page.getValues(offset + 1, index), vals, index))
                offset++;
        else
            while (offset + 1 < size && !less(vals, page.getValues(offset + 1, index), index))
                offset++;
        if (page.getIdent() == RECORD)
        {
            ret = Pos(pageID, offset);
            break;
        }
        int childID = dynamic_cast<IntType*>(page.getValue(offset, "$child").get())->getVal();
        if (getDataPage(childID).getIdent() == REF)
        {
            ret = Pos(pageID, offset);
            break;
        }
        pageID = childID;
    }
    if (equal)
    {
        if (less(getDataPage(ret.first).getValues(ret.second, index), vals, index))
            ret.second++;
    } else
        if (!less(vals, getDataPage(ret.first).getValues(ret.second, index), index))
            ret.second++;
    return ret;
}

void BaseTable::selectLinear(
    std::vector<ColVal> &ret,
    const BaseTable::Index &targets, const BaseTable::ConsVal &constraints, const BaseTable::Pos &start,
    const BaseTable::ColVal &stopV, const BaseTable::Index &stopIdx, bool stopEq
)
{
    int pageID = start.first;
    while (true)
    {
        ListPage &page = getDataPage(pageID);
        assert(page.getIdent() == RECORD);
        int st = pageID == start.first ? start.second : 0;
        int en = page.getSize();
        for (int i = st; i < en; i++)
        {
            if (meetCons(page, i, constraints))
            {
                ret.push_back(page.getValues(i, targets));
                if (!primary.isOk()) // Useful for non-clustered indexes
                {
                    auto aux = Type::newType(Type::INT);
                    dynamic_cast<IntType*>(aux.get())->setVal(pageID);
                    ret.back()["$page"] = std::move(aux);
                }
            }
            if (stopEq && less(stopV, page.getValues(i, stopIdx), stopIdx))
                return;
            if (!stopEq && !less(page.getValues(i, stopIdx), stopV, stopIdx))
                return;
        }
        int nextID = page.getNext();
        if (!~nextID) return;
        pageID = nextID;
    }
}

void BaseTable::selectRefLinear(
    std::vector<ColVal> &ret,
    const BaseTable::Index &targets, const BaseTable::ConsVal &constraints, const BaseTable::Pos &start,
    const BaseTable::ColVal &stopV, const BaseTable::Index &stopIdx, bool stopEq
)
{
    int pageID = start.first;
    std::set<int> visitedPages;
    while (true)
    {
        ListPage &page = getDataPage(pageID);
        assert(page.getIdent() >= NON_CLUSTER);
        int st = pageID == start.first ? start.second : 0;
        int en = page.getSize();
        for (int i = st; i < en; i++)
        {
            int childID = dynamic_cast<IntType*>(page.getValue(i, "$child").get())->getVal();
            while (childID != -1)
            {
                ListPage &refPage = getDataPage(childID);
                assert(refPage.getIdent() == REF);
                for (int j = 0; j < refPage.getSize(); j++)
                    if (primary.isOk())
                    {
                        auto key = refPage.getValues(j, primary.ok());
                        Pos item = findFirst(0, key, primary.ok(), true);
                        ret.push_back(getDataPage(item.first).getValues(item.second, targets));
                    } else
                    {
                        int key = dynamic_cast<IntType*>(refPage.getValue(j, "$page").get())->getVal();
                        if (!visitedPages.count(key))
                        {
                            visitedPages.insert(key);
                            ListPage &recPage = getDataPage(key);
                            assert(recPage.getIdent() == RECORD);
                            for (int k = 0; k < recPage.getSize(); k++)
                                if (meetCons(recPage, k, constraints))
                                    ret.push_back(recPage.getValues(k, targets));
                        }
                    }
                childID = refPage.getNext();
            }
            if (stopEq && less(stopV, page.getValues(i, stopIdx), stopIdx))
                return;
            if (!stopEq && !less(page.getValues(i, stopIdx), stopV, stopIdx))
                return;
        }
        int nextID = page.getNext();
        if (!~nextID) return;
        pageID = nextID;
    }
}

void BaseTable::rotateRoot(int rootID, int newChildRID)
{
    ListPage &root = getDataPage(rootID);
    int newChildLID = newDataPage(root.getIdent());
    ListPage &newChildL = getDataPage(newChildLID);
    ListPage &newChildR = getDataPage(newChildRID);
    newChildL.setSize(root.getSize());
    for (int i = 0; i < root.getSize(); i++)
        ListPage::copy(&root, i, &newChildL, i);
    destroyDataPage(rootID);
    int newID = newDataPage(PRIMARY); // change Ident of 0 to PRIMARY
    assert(newID == rootID);

    ColVal newItemL = newChildL.getValues(0, primary.ok());
    newItemL["$child"] = Type::newType(Type::INT);
    dynamic_cast<IntType*>(newItemL["$child"].get())->setVal(newChildLID);
    root.setValues(0, newItemL);
    // destroyPage won't destory the page object, so no need to reload `root`

    ColVal newItemR = newChildR.getValues(0, primary.ok());
    newItemR["$child"] = Type::newType(Type::INT);
    dynamic_cast<IntType*>(newItemR["$child"].get())->setVal(newChildRID);
    root.setValues(1, newItemR);

    newChildL.setPrev(-1), newChildL.setNext(newChildRID);
    newChildR.setNext(-1), newChildR.setPrev(newChildLID);

    root.setSize(2);
}

BaseTable::Bound BaseTable::getBound(const BaseTable::ConsVal &constraints, const BaseTable::Index &index)
{
    ColVal l, r;
    bool openL = false, openR = false; // open interval
    for (const std::string &name : index)
    {
        bool done = true;
        if (constraints.count(name))
            for (const auto &item : constraints.at(name))
                switch (item.dir)
                {
                    // Don't care about redundent or conflictory consitions
                    // The linear scanning guarentees the correctness
                case EQ:
                    l[name] = Type::newFromCopy(item.pivot);
                    r[name] = Type::newFromCopy(item.pivot);
                    openL = openR = false;
                    done = false;
                    break;
                case LE:
                    openR = false;
                    r[name] = Type::newFromCopy(item.pivot);
                    break;
                case LT:
                    openR = true;
                    r[name] = Type::newFromCopy(item.pivot);
                    break;
                case GE:
                    openL = false;
                    l[name] = Type::newFromCopy(item.pivot);
                    break;
                case GT:
                    openL = true;
                    l[name] = Type::newFromCopy(item.pivot);
                    break;
                default:
                    ; // leave it
                }
        if (done)
            break;
    }
    return std::make_pair(std::make_pair(std::move(l), openL), std::make_pair(std::move(r), openR));
}

void BaseTable::insert(const BaseTable::ColVal &vals)
{
    ColVal valWithPos;
    for (const auto &pair : vals)
        valWithPos[pair.first] = Type::newFromCopy(pair.second);
    if (primary.isOk())
    {
        int newChildRID = insertRecur(0, vals, primary.ok());
        if (newChildRID != -1) // root should still be page 0
            rotateRoot(0, newChildRID);
    } else
    {
        int pageID = insertLinear(0, vals);
        auto pageIDCol = Type::newType(Type::INT);
        dynamic_cast<IntType*>(pageIDCol.get())->setVal(pageID);
        valWithPos["$page"] = std::move(pageIDCol);
    }
    for (int i = 0; i < int(nonClus.size()); i++)
    {
        int newChildRID = insertRecur(i + 1, valWithPos, nonClus[i]);
        if (newChildRID != -1)
            rotateRoot(i + 1, newChildRID);
    }
}

void BaseTable::remove(const ConsVal &constraints)
{
    ListPage &root = getDataPage(0);
    if (primary.isOk())
        for (const auto &record : select(primary.ok(), constraints))
        {
            removeRecur(0, record, primary.ok());
            if (root.getIdent() == PRIMARY && root.getSize() == 1)
            {
                int childID = dynamic_cast<IntType*>(root.getValue(0, "$child").get())->getVal();
                ListPage &child = getDataPage(childID);
                root.setSize(child.getSize());
                for (int i = 0; i < root.getSize(); i++)
                    ListPage::copy(&child, i, &root, i);
                destroyDataPage(childID);
            }
        }
    else
        removeLinear(0, constraints, false);
    // TODO : remove from non-clustered indexes
}

std::vector<BaseTable::ColVal> BaseTable::select(const BaseTable::Index &targets, const ConsVal &constraints)
{
    std::vector<BaseTable::ColVal> ret;

    int best = -1; // best index ID. -1 for primary (or none, if there is no primary)
    ColVal l, r;
    bool openL = false, openR = false;
    if (primary.isOk())
    {
        auto bound = getBound(constraints, primary.ok());
        l = std::move(bound.first.first), r = std::move(bound.second.first);
        openL = bound.first.second, openR = bound.second.second;
    }
    for (int i = 0; i < int(nonClus.size()); i++)
    {
        auto bound = getBound(constraints, nonClus[i]);
        assert(bound.first.first.size() == bound.second.first.size());
        if (bound.first.first.size() <= l.size())
            continue;
        l = std::move(bound.first.first), r = std::move(bound.second.first);
        openL = bound.first.second, openR = bound.second.second;
        best = i;
    }
    if (!~best && !primary.isOk())
        selectLinear(ret, targets, constraints);
    else
    {
        // NOTE: l.size() can still be 0 here
        const Index &index = best >= 0 ? nonClus[best] : primary.ok();
        Index indexL, indexR;
        for (const std::string &name : index)
        {
            if (l.count(name)) indexL.push_back(name);
            if (r.count(name)) indexR.push_back(name);
        }
        Pos start = findFirst(best + 1, l, indexL, !openL); // ( : first >, [ : first >=
        if (!~best)
            selectLinear(ret, targets, constraints, start, r, indexR, !openR);
        else
            selectRefLinear(ret, targets, constraints, start, r, indexR, !openR);
    }
    return ret;
}

