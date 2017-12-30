#include <cassert>
#include <set>
#include "utils.h"
#include "BaseTable.h"
#include "type/IntType.h"

BaseTable::BaseTable
(
    PageCache &_cache, const std::string &_tableName, const Cols &_cols,
    const Optional<Index> &_primary, const std::vector<Index> &_nonClus
)
    : TablePages(_cache, _tableName, _cols, _primary, _nonClus),
      primary(_primary), nonClus(_nonClus)
{
    allColumns.reserve(_cols.size());
    for (const auto &pair : _cols)
        allColumns.push_back(pair.first);

    // Initialize entry page
    if (!isFree(ENTRY_PAGE)) newDataPage(ENTRY);
    // i = 0 is for primary entry, i > 0 is for non-cluster entry
    for (int i = getDataPage(ENTRY_PAGE).getSize(); i <= int(_nonClus.size()); i++)
    {
        int pageID = newDataPage(i ? NON_CLUSTER + i - 1: RECORD);
        getDataPage(ENTRY_PAGE).setValue(i, "$page", Type::newFromLiteral(std::to_string(pageID), Type::INT));
    }
    getDataPage(ENTRY_PAGE).setSize(nonClus.size() + 1);
}

int BaseTable::priEntry()
{
    return dynamic_cast<IntType*>(getDataPage(ENTRY_PAGE).getValue(0, "$page").get())->getVal();
}

void BaseTable::setPriEntry(int e)
{
    ColVal newItem;
    newItem["$page"] = Type::newType(Type::INT);
    dynamic_cast<IntType*>(newItem["$page"].get())->setVal(e);
    getDataPage(ENTRY_PAGE).setValues(0, std::move(newItem));
}

int BaseTable::ncEntry(int indexID)
{
    assert(indexID + 1 >= 1 && indexID + 1 < getDataPage(ENTRY_PAGE).getSize());
    return dynamic_cast<IntType*>(getDataPage(ENTRY_PAGE).getValue(indexID + 1, "$page").get())->getVal();
}

void BaseTable::setNcEntry(int indexID, int e)
{
    assert(indexID + 1 >= 1 && indexID + 1 < getDataPage(ENTRY_PAGE).getSize());
    ColVal newItem;
    newItem["$page"] = Type::newType(Type::INT);
    dynamic_cast<IntType*>(newItem["$page"].get())->setVal(e);
    getDataPage(ENTRY_PAGE).setValues(indexID + 1, std::move(newItem));
}

void BaseTable::delNcEntry(int indexID)
{
    assert(indexID + 1 >= 1 && indexID + 1 < getDataPage(ENTRY_PAGE).getSize());
    ListPage &entry = getDataPage(ENTRY_PAGE);
    entry.setSize(entry.getSize() - 1);
    for (int i = indexID + 1; i < entry.getSize(); i++)
        entry.copy(i + 1, i);
}

bool BaseTable::meetCons(ListPage &page, int rank, const BaseTable::ConsVal &cons, const BaseTable::OuterCons &oCons) const
{
    for (const auto &pair : cons)
    {
        const std::string &name = pair.first;
        for (const ConValue &rhs : pair.second)
        {
            std::unique_ptr<Type> lhs = page.getValue(rank, name);
            switch (rhs.dir)
            {
            case IS_NULL:
                assert(rhs.pivot == nullptr);
                if (lhs != nullptr) return false;
                break;
            case IS_NOT_NULL:
                assert(rhs.pivot == nullptr);
                if (lhs == nullptr) return false;
                break;
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
    for (const auto &item : oCons)
    {
        std::unique_ptr<Type> lhs = page.getValue(rank, item.col1);
        std::unique_ptr<Type> rhs = page.getValue(rank, item.col2);
        if (lhs == nullptr || rhs == nullptr) return false;
        switch (item.dir)
        {
        case EQ:
            if (!(*lhs == *rhs)) return false;
            break;
        case NE:
            if (!(*lhs != *rhs)) return false;
            break;
        case LT:
            if (!(*lhs < *rhs)) return false;
            break;
        case LE:
            if (!(*lhs <= *rhs)) return false;
            break;
        case GT:
            if (!(*lhs > *rhs)) return false;
            break;
        case GE:
            if (!(*lhs >= *rhs)) return false;
            break;
        default: assert(false);
        }
    }
    return true;
}

void BaseTable::updNode(int pageID, int offset, const BaseTable::Index &index)
{
    ListPage &page = getDataPage(pageID);
    int childID = dynamic_cast<IntType*>(page.getValue(offset, "$child").get())->getVal();
    page.setValues(offset, getDataPage(childID).getValuesNow(0, index));
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
    int offset = page.getSize() - 1; // In most cases, the latest data has the largest ID
    if (offset >= 0 && less(vals, page.getValues(offset), index))
        offset = binSearch(0, size, [&vals, &page, &index](int id) {
            return !less(vals, page.getValues(id), index);
        }) - 1;
    // `offset` is the last that <= `vals`

    if (page.getIdent() == RECORD) // leaf node of primary index
    {
        if (offset >= 0 && equal(page.getValues(offset), vals, index))
            throw NotUniqueException(index);
        return insertAndSplit(pageID, vals, offset + 1);
    }

    if ( // leaf node of non-clustered index
        size == 0 || // newly created NON_CLUSTER node
        getDataPage(dynamic_cast<IntType*>(page.getValue(0, "$child").get())->getVal()).getIdent() == REF
    )
    {
        ColVal newRef;
        if (primary.isOk())
            for (const std::string &name : primary.ok())
                newRef[name] = Type::newFromCopy(vals.at(name));
        else
            newRef["$page"] = Type::newFromCopy(vals.at("$page"));
        if (offset >= 0 && equal(page.getValues(offset), vals, index))
        {
            // insert into the ref page
            int childID = dynamic_cast<IntType*>(page.getValue(offset, "$child").get())->getVal();
            insertLinear(childID, newRef, REF);
            return -1;
        } else
        {
            // generate a new ref page and insert as a child
            int childID = newDataPage(REF);
            insertLinear(childID, newRef, REF);
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
        page.setValues(0, getDataPage(childID).getValuesNow(0, index));
    int newChildID = insertRecur(childID, vals, index);
    if (!~newChildID)
        return -1;
    ColVal newItem = getDataPage(newChildID).getValuesNow(0, index);
    newItem["$child"] = Type::newType(Type::INT);
    dynamic_cast<IntType*>(newItem["$child"].get())->setVal(newChildID);
    return insertAndSplit(pageID, newItem, offset + 1);
}

int BaseTable::insertLinear(int pageID, const BaseTable::ColVal &vals, short ident)
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
            ListPage &next = getDataPage(nextID = newDataPage(ident));
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
    int offset = binSearch(0, size, [&vals, &page, &index](int id) {
        return !less(vals, page.getValues(id), index);
    }) - 1;
    // `offset` is the last that <= `vals`
    if (!~offset) return None();

    if (page.getIdent() == RECORD)
    {
        assert(equal(page.getValues(offset), vals, index)); // becase we are removing selected items
        return removeAndMerge(pageID, offset);
    }
    int childID = dynamic_cast<IntType*>(page.getValue(offset, "$child").get())->getVal();
    ListPage &child = getDataPage(childID);
    if (child.getIdent() == REF)
    {
        assert(equal(page.getValues(offset), vals, index)); // becase we are removing selected items
        ConsVal c;
        for (const std::string &name : primary.isOk() ? primary.ok() : Index({"$page"}))
        {
            std::vector<ConValue> v;
            v.push_back({(ConValue){EQ, Type::newFromCopy(vals.at(name))}});
            c[name] = std::move(v); // move sementic is not supported in initializer_list
        }
        if (removeLinear(childID, std::move(c), {}, true))
        {
            destroyDataPage(childID);
            return removeAndMerge(pageID, offset);
        }
        return None();
    }

    // recurse
    auto needRemove = removeRecur(childID, vals, index);
    if (needRemove.isOk())
        return removeAndMerge(pageID, offset + needRemove.ok());
    updNode(pageID, offset, index); // Rotation will destroy original data
    if (offset + 1 < page.getSize())
        updNode(pageID, offset + 1, index);
    return None();
}

bool BaseTable::removeLinear(int pageID, const ConsVal &constraints, const OuterCons &oCons, bool onlyOne)
{
    const int startID = pageID;
    assert(getDataPage(startID).getPrev() == -1);
    bool removedOne = false, empty = false;
    while (true)
    {
        ListPage &page = getDataPage(pageID);
        int oldSize = page.getSize(), size = oldSize;
        for (int i = 0, j = 0; i < oldSize; i++)
            if (meetCons(page, i, constraints, oCons) && (!removedOne || !onlyOne))
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
                    ListPage::copy(&getDataPage(nextID), &page);
                    page.setPrev(-1);
                    std::swap(pageID, nextID);
                }
            }
            if (!empty)
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
        int offset = equal ?
                binSearch(1, size, [&vals, &page, &index](int id) {
                    return less(page.getValues(id), vals, index);
                }) - 1 // Yes, -1 here because we will recurse
            :
                binSearch(1, size, [&vals, &page, &index](int id) {
                    return !less(vals, page.getValues(id), index);
                }) - 1;
        if (page.getIdent() == RECORD)
        {
            ret = Pos(pageID, offset);
            break;
        }
        if (!size) // newly created NON_CLUSTER node
        {
            ret = Pos(pageID, 0);
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
    ListPage &recPage = getDataPage(ret.first);
    if (ret.second < recPage.getSize())
    {
        if (equal)
        {
            if (less(getDataPage(ret.first).getValues(ret.second), vals, index))
                ret.second++;
        } else
            if (!less(vals, getDataPage(ret.first).getValues(ret.second), index))
                ret.second++;
    }
    assert(ret.second <= recPage.getSize());
    if (ret.second == recPage.getSize())
        ret = Pos(recPage.getNext(), 0);
    return ret;
}

void BaseTable::addToSelection(
    std::vector<BaseTable::ColVal> &ret, int pageID, int off, const Index &targets,
    const BaseTable::ConsVal &constraints, const OuterCons &oCons
)
{
    ListPage &page = getDataPage(pageID);
    if (meetCons(page, off, constraints, oCons))
    {
        ret.push_back(page.getValuesNow(off, targets));
        if (!primary.isOk()) // Useful for non-clustered indexes removal
        {
            auto aux = Type::newType(Type::INT);
            dynamic_cast<IntType*>(aux.get())->setVal(pageID);
            ret.back()["$page"] = std::move(aux);
        }
    }
}

void BaseTable::selectLinear(
    std::vector<ColVal> &ret,
    const BaseTable::Index &targets, const BaseTable::ConsVal &constraints, const OuterCons &oCons,
    const BaseTable::Pos &start, const BaseTable::ColVal &stopV, const BaseTable::Index &stopIdx, bool stopEq
)
{
    int pageID = start.first; // NOTE: page ID might be -1 return by findFirst
    while (pageID != -1)
    {
        ListPage &page = getDataPage(pageID);
        assert(page.getIdent() == RECORD);
        int st = pageID == start.first ? start.second : 0;
        int en = page.getSize();
        for (int i = st; i < en; i++)
        {
            addToSelection(ret, pageID, i, targets, constraints, oCons);
            if (stopEq && less(stopV, page.getValues(i), stopIdx))
                return;
            if (!stopEq && !less(page.getValues(i), stopV, stopIdx))
                return;
        }
        pageID = page.getNext();
    }
}

void BaseTable::selectRefLinear(
    std::vector<ColVal> &ret,
    const BaseTable::Index &targets, const BaseTable::ConsVal &constraints, const OuterCons &oCons,
    const BaseTable::Pos &start, const BaseTable::ColVal &stopV, const BaseTable::Index &stopIdx, bool stopEq
)
{
    int pageID = start.first; // NOTE: page ID might be -1 return by findFirst
    std::set<int> visitedPages;
    while (pageID != -1)
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
                        auto key = refPage.getValuesNow(j, primary.ok());
                        Pos item = findFirst(priEntry(), key, primary.ok(), true);
                        assert(item.first >= 0); // This item must exist
                        ListPage &recPage = getDataPage(item.first);
                        if (meetCons(recPage, item.second, constraints, oCons))
                            ret.push_back(recPage.getValuesNow(item.second, targets));
                    } else
                    {
                        int key = dynamic_cast<IntType*>(refPage.getValue(j, "$page").get())->getVal();
                        if (!visitedPages.count(key))
                        {
                            visitedPages.insert(key);
                            ListPage &recPage = getDataPage(key);
                            assert(recPage.getIdent() == RECORD);
                            for (int k = 0; k < recPage.getSize(); k++)
                                addToSelection(ret, key, k, targets, constraints, oCons);
                        }
                    }
                childID = refPage.getNext();
            }
            if (stopEq && less(stopV, page.getValues(i), stopIdx))
                return;
            if (!stopEq && !less(page.getValues(i), stopV, stopIdx))
                return;
        }
        pageID = page.getNext();
    }
}

void BaseTable::rotateRoot(int newChildLID, int newChildRID, const Index &index, short ident)
{
    ListPage &newChildL = getDataPage(newChildLID);
    ListPage &newChildR = getDataPage(newChildRID);
    int rootID = newDataPage(ident); // May need to change from RECORD to PRIMARY
    ListPage &root = getDataPage(rootID);
    if (ident >= NON_CLUSTER)
        setNcEntry(ident - NON_CLUSTER, rootID);
    else
        setPriEntry(rootID);

    ColVal newItemL = newChildL.getValuesNow(0, index);
    newItemL["$child"] = Type::newType(Type::INT);
    dynamic_cast<IntType*>(newItemL["$child"].get())->setVal(newChildLID);
    root.setValues(0, newItemL);

    ColVal newItemR = newChildR.getValuesNow(0, index);
    newItemR["$child"] = Type::newType(Type::INT);
    dynamic_cast<IntType*>(newItemR["$child"].get())->setVal(newChildRID);
    root.setValues(1, newItemR);

    newChildL.setPrev(-1), newChildL.setNext(newChildRID);
    newChildR.setNext(-1), newChildR.setPrev(newChildLID);

    root.setSize(2);
}

void BaseTable::removeRoot(int rootID)
{
    ListPage &root = getDataPage(rootID);
    int childID = dynamic_cast<IntType*>(root.getValue(0, "$child").get())->getVal();
    const short ident = root.getIdent();
    if (ident == RECORD || getDataPage(childID).getIdent() == REF)
        return;
    if (ident >= NON_CLUSTER)
        setNcEntry(ident - NON_CLUSTER, childID);
    else
        setPriEntry(childID);
    destroyDataPage(rootID);
}

void BaseTable::updLBound(BaseTable::ColVal &l, const std::string &name, bool &open, const std::unique_ptr<Type> &_l, bool _open)
{
    if (!l.count(name) || Type::less(l.at(name), _l) || (Type::equal(_l, l.at(name)) && !open && _open))
        l[name] = Type::newFromCopy(_l), open = _open;
}

void BaseTable::updRBound(BaseTable::ColVal &r, const std::string &name, bool &open, const std::unique_ptr<Type> &_r, bool _open)
{
    if (!r.count(name) || Type::less(_r, r.at(name)) || (Type::equal(_r, r.at(name)) && !open && _open))
        r[name] = Type::newFromCopy(_r), open = _open;
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
                case IS_NULL:
                    updRBound(r, name, openR, nullptr, false);
                    done = false;
                    break;
                case IS_NOT_NULL:
                    updLBound(l, name, openL, nullptr, true);
                    break;
                case EQ:
                    updLBound(l, name, openL, item.pivot, false);
                    updRBound(r, name, openR, item.pivot, false);
                    done = false;
                    break;
                case LE:
                    updLBound(l, name, openL, nullptr, true);
                    updRBound(r, name, openR, item.pivot, false);
                    break;
                case LT:
                    updLBound(l, name, openL, nullptr, true);
                    updRBound(r, name, openR, item.pivot, true);
                    break;
                case GE:
                    updLBound(l, name, openL, item.pivot, false);
                    break;
                case GT:
                    updLBound(l, name, openL, item.pivot, true);
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
        int newChildRID = insertRecur(priEntry(), vals, primary.ok());
        if (newChildRID != -1) // root should still be page 0
            rotateRoot(priEntry(), newChildRID, primary.ok(), PRIMARY);
    } else
    {
        int pageID = insertLinear(priEntry(), vals, RECORD);
        auto pageIDCol = Type::newType(Type::INT);
        dynamic_cast<IntType*>(pageIDCol.get())->setVal(pageID);
        valWithPos["$page"] = std::move(pageIDCol);
    }
    for (int i = 0; i < int(nonClus.size()); i++)
    {
        int newChildRID = insertRecur(ncEntry(i), valWithPos, nonClus[i]);
        if (newChildRID != -1)
            rotateRoot(ncEntry(i), newChildRID, nonClus[i], getDataPage(ncEntry(i)).getIdent());
    }
}

void BaseTable::remove(const ConsVal &constraints, const OuterCons &oCons)
{
    Optional< std::vector<ColVal> > toDel;
    if (primary.isOk() || !nonClus.empty())
        toDel = select(allColumns, constraints, oCons);
    if (primary.isOk())
        for (const auto &record : toDel.ok())
        {
            ListPage &root = getDataPage(priEntry()); // This line placed here to be updated after `removeRoot` below
            removeRecur(priEntry(), record, primary.ok());
            if (root.getIdent() == PRIMARY && root.getSize() == 1)
                removeRoot(priEntry());
        }
    else
        removeLinear(priEntry(), constraints, oCons, false);

    for (int i = 0; i < int(nonClus.size()); i++)
        for (const auto &record : toDel.ok())
        {
            ListPage &root = getDataPage(ncEntry(i)); // This line placed here to be updated after `removeRoot` below
            removeRecur(ncEntry(i), record, nonClus[i]);
            if (
                root.getSize() == 1 &&
                getDataPage(dynamic_cast<IntType*>(root.getValue(0, "$child").get())->getVal()).getIdent() != REF
            )
                removeRoot(ncEntry(i));
        }
}

std::vector<BaseTable::ColVal> BaseTable::select(
    const BaseTable::Index &targets, const ConsVal &constraints, const OuterCons &oCons
)
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
        if (std::min(bound.first.first.size(), bound.second.first.size()) <= std::min(l.size(), r.size()))
            continue;
        l = std::move(bound.first.first), r = std::move(bound.second.first);
        openL = bound.first.second, openR = bound.second.second;
        best = i;
    }
    if (!~best && !primary.isOk())
        selectLinear(ret, targets, constraints, oCons, Pos(priEntry(), 0));
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
        Pos start = findFirst(!~best ? priEntry() : ncEntry(best), l, indexL, !openL); // ( : first >, [ : first >=
        if (!~best)
            selectLinear(ret, targets, constraints, oCons, start, r, indexR, !openR);
        else
            selectRefLinear(ret, targets, constraints, oCons, start, r, indexR, !openR);
    }
    return ret;
}

int BaseTable::addIndex(const BaseTable::Index &index)
{
    // Update entry
    int indexID = nonClus.size();
    nonClus.push_back(index);
    registerNewIndex(index); // Update `TablePages` This line should before newDataPage
    getDataPage(ENTRY_PAGE).setSize(getDataPage(ENTRY_PAGE).getSize() + 1);
    setNcEntry(indexID, newDataPage(NON_CLUSTER, indexID));

    // Build new index tree
    // Don't select all and then insert, which will consume lots of memory
    for (int pageID = priEntry(); pageID != -1;)
    {
        ListPage &page = getDataPage(pageID);
        assert(page.getIdent() == RECORD);
        for (int i = 0; i < page.getSize(); i++)
        {
            auto valWithPos = page.getValuesNow(i, allColumns);
            if (!primary.isOk())
            {
                valWithPos["$page"] = Type::newType(Type::INT);
                dynamic_cast<IntType*>(valWithPos["$page"].get())->setVal(pageID);
            }
            int newChildRID = insertRecur(ncEntry(indexID), std::move(valWithPos), index);
            if (newChildRID != -1)
                rotateRoot(ncEntry(indexID), newChildRID, index, getDataPage(ncEntry(indexID)).getIdent());
        }
        pageID = page.getNext();
    }
    return indexID;
}

void BaseTable::delIndexRecur(int pageID)
{
    ListPage *page = &getDataPage(pageID);
    if (page->getIdent() == REF)
    {
        while (!~pageID)
        {
            page = &getDataPage(pageID);
            int nextID = page->getNext();
            destroyDataPage(pageID);
            pageID = nextID;
        }
        return;
    }
    assert(page->getIdent() >= NON_CLUSTER);
    for (int i = 0; i < page->getSize(); i++)
    {
        int childID = dynamic_cast<IntType*>(page->getValue(i, "$child").get())->getVal();
        delIndexRecur(childID);
    }
    destroyDataPage(pageID);
}

void BaseTable::delIndex(int indexID)
{
    delIndexRecur(ncEntry(indexID));
    delNcEntry(indexID);
    nonClus.erase(nonClus.begin() + indexID);
    registerDelIndex(indexID);
}

