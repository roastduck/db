#include <cassert>
#include "ListPage.h"
#include "exception/NotNullException.h"
#include "exception/SameNameException.h"
#include "exception/NotInDomainException.h"

ListPage::ListPage(PageCache &_pageCache, const std::string &_filename, int _pageID, const std::vector<Column> &_cols)
    : BasePage(_pageCache, _filename, _pageID)
{
    for (const Column &col : _cols)
    {
        std::unique_ptr<Type> type = Type::newType(col.typeID, col.length);
        OffsetColumn off;
        off.col = col;
        off.posOffset = fixedBytes, fixedBytes += type->getFixedLength();
        off.nullOffset = col.notNull ? -1 : nullCnt++;
        bool success = cols.insert(std::make_pair(col.name, std::move(off))).second;
        if (!success)
            throw SameNameException(col.name);
    }
    nullBytes = (nullCnt + 7) / 8;
    recBytes = fixedBytes + nullBytes;
}

std::unique_ptr<Type> ListPage::getValue(int rank, const std::string &name)
{
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

void ListPage::setValue(int rank, const std::string &name, const std::unique_ptr<Type> &value)
{
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
        bool inDomain = off.col.allowedDomain.empty();
        for (auto i = off.col.allowedDomain.begin(); !inDomain && i != off.col.allowedDomain.end(); i++)
            inDomain = (*Type::newFromLiteral(*i, value->getTypeID(), value->getLength()) == *value);
        if (!inDomain)
            throw NotInDomainException(value, name);
        auto bytes = value->toBytes();
        std::copy(bytes.begin(), bytes.end(), accessMut(rank) + nullBytes + off.posOffset);
    }
}

void ListPage::copy(int rank1, int rank2)
{
    std::copy(accessConst(rank1), accessConst(rank1) + recBytes, accessMut(rank2));
}

