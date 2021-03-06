#include <cassert>
#include <stdexcept>
#include "ListPage.h"
#include "../config.h"
#include "../VectorRef.h"
#include "../exception/NotNullException.h"
#include "../exception/RecordTooLargeException.h"

void ListPage::setCols(const std::unordered_map<std::string, Column> &_cols)
{
    nullCnt = 0, fixedBytes = 0;
    for (const auto &pair : _cols)
    {
        const std::string name = pair.first;
        const Column &col = pair.second;
        std::unique_ptr<Type> type = Type::newType(col.typeID, col.length);
        OffsetColumn off;
        off.col = col;
        off.posOffset = fixedBytes, fixedBytes += type->getFixedLength();
        off.nullOffset = col.notNull ? -1 : nullCnt++;
        cols[name] = std::move(off);
    }
    nullBytes = ((nullCnt + 7) >> 3);
    recBytes = fixedBytes + nullBytes;
    if (recBytes > MAX_RECORD_BYTES)
        throw RecordTooLargeException(recBytes);
}

std::unique_ptr<Type> ListPage::getValue(int rank, const std::string &name)
{
    try
    {
        assert(rank >= 0 && rank < getMaxSize());
        const OffsetColumn &off = cols.at(name);
        if (!off.col.notNull && getNull(rank, off.nullOffset))
            return nullptr;
        std::unique_ptr<Type> ret = Type::newType(off.col.typeID, off.col.length);
        ret->fromBytes(VectorRef<unsigned char>(
            accessConst(rank) + nullBytes + off.posOffset, ret->getFixedLength()
        ));
        return ret;
    } catch (const std::out_of_range &e)
    {
        throw NoSuchThingException("field", name);
    }
}

std::unordered_map< std::string, std::unique_ptr<Type> > ListPage::getValuesNow(int rank, const std::vector<std::string> &names)
{
    assert(rank >= 0 && rank < getMaxSize());
    std::unordered_map< std::string, std::unique_ptr<Type> > ret;
    ret.max_load_factor(0.33);
    ret.reserve(names.size());
    for (const std::string &name : names)
        ret[name] = getValue(rank, name);
    return ret;
}

std::vector<std::pair<std::string, std::unique_ptr<Type>>> ListPage::getValuesLst(int rank, const std::vector<std::string> &names)
{
    assert(rank >= 0 && rank < getMaxSize());
    std::vector<std::pair<std::string, std::unique_ptr<Type>>> ret;
    ret.reserve(names.size());
    for (const std::string &name : names)
        ret.emplace_back(name, getValue(rank, name));
    return ret;
}

LazyMap<std::string, std::unique_ptr<Type>> ListPage::getValues(int rank)
{
    assert(rank >= 0 && rank < getMaxSize());
    return LazyMap<std::string, std::unique_ptr<Type>>([this, rank](const std::string &name){
        return getValue(rank, name);
    });
}

void ListPage::setValue(int rank, const std::string &name, const std::unique_ptr<Type> &value)
{
    try
    {
        assert(rank >= 0 && rank < getMaxSize());
        const OffsetColumn &off = cols.at(name);
        if (value == nullptr)
            if (off.col.notNull)
                throw NotNullException(name);
            else
                setNull(rank, off.nullOffset, true);
        else
        {
            assert(value->getTypeID() == off.col.typeID);
            if (!off.col.notNull)
                setNull(rank, off.nullOffset, false);
            auto bytes = value->toBytes();
            std::copy(bytes.begin(), bytes.end(), accessMut(rank) + nullBytes + off.posOffset);
        }
    } catch (const std::out_of_range &e)
    {
        throw NoSuchThingException("field", name);
    }
}

void ListPage::copy(int rank1, int rank2)
{
    if (rank1 != rank2)
        std::copy(accessConst(rank1), accessConst(rank1) + recBytes, accessMut(rank2));
}

void ListPage::copy(ListPage *page1, int rank1, ListPage *page2, int rank2)
{
    assert(page1->recBytes == page2->recBytes);
    std::copy(page1->accessConst(rank1), page1->accessConst(rank1) + page1->recBytes, page2->accessMut(rank2));
}

void ListPage::copy(ListPage *page1, ListPage *page2)
{
    page2->setSize(page1->getSize());
    page2->setNext(page1->getNext());
    page2->setPrev(page1->getPrev());
    for (int i = 0; i < page1->getSize(); i++)
        copy(page1, i, page2, i);
}

