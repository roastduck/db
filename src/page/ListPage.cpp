#include <cassert>
#include "ListPage.h"
#include "../exception/NotNullException.h"

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
}

std::unique_ptr<Type> ListPage::getValue(int rank, const std::string &name)
{
    assert(rank >= 0 && rank < getMaxSize());
    const OffsetColumn &off = cols.at(name);
    if (!off.col.notNull && getNull(rank, off.nullOffset))
        return nullptr;
    std::unique_ptr<Type> ret = Type::newType(off.col.typeID, off.col.length);
    ret->fromBytes(std::vector<unsigned char>(
        accessConst(rank) + nullBytes + off.posOffset,
        accessConst(rank) + nullBytes + off.posOffset + ret->getFixedLength()
    ));
    return ret;
}

std::unordered_map< std::string, std::unique_ptr<Type> > ListPage::getValues(int rank, const std::vector<std::string> &names)
{
    assert(rank >= 0 && rank < getMaxSize());
    std::unordered_map< std::string, std::unique_ptr<Type> > ret;
    for (const std::string &name : names)
        ret[name] = getValue(rank, name);
    return ret;
}

void ListPage::setValue(int rank, const std::string &name, const std::unique_ptr<Type> &value)
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
}

void ListPage::setValues(int rank, const std::unordered_map< std::string, std::unique_ptr<Type> > &values)
{
    for (const auto &pair : values)
        setValue(rank, pair.first, pair.second);
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

