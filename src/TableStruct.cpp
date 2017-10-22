#include <cassert>
#include "RawRecord.h"
#include "TableStruct.h"
#include "exception/NotNullException.h"
#include "exception/SameNameException.h"
#include "exception/NotInDomainException.h"

TableStruct::TableStruct(const std::vector<Column> &_cols)
    : fixedBytes(0), nullCnt(0), varCnt(0)
{
    for (const Column &col : _cols)
    {
        std::unique_ptr<Type> type = Type::newType(col.typeID, col.length);
        OffsetColumn off;
        off.col = col;
        if (type->getFixedLength())
            off.posOffset = fixedBytes, fixedBytes += type->getFixedLength();
        else
            off.posOffset = varCnt++;
        off.nullOffset = col.notNull ? -1 : nullCnt++;
        bool success = cols.insert(std::make_pair(col.name, std::move(off))).second;
        if (!success)
            throw SameNameException(col.name);
    }
}

std::unique_ptr<Type> TableStruct::getValue(const RawRecord &raw, const std::string &name) const
{
    const OffsetColumn &off = cols.at(name);
    if (!off.col.notNull && raw.nullColumns[off.nullOffset])
        return nullptr;
    std::unique_ptr<Type> ret = Type::newType(off.col.typeID, off.col.length);
    if (ret->getFixedLength())
        ret->fromBytes(std::vector<unsigned char>(
            raw.fixedLengths.begin() + off.posOffset,
            raw.fixedLengths.begin() + off.posOffset + ret->getFixedLength()
        ));
    else
        ret->fromBytes(raw.varLengths[off.posOffset]);
    return ret;
}

void TableStruct::setValue(RawRecord &raw, const std::string &name, const std::unique_ptr<Type> &value) const
{
    if (raw.nullColumns.empty())
        raw.nullColumns.resize(nullCnt);
    if (raw.fixedLengths.empty())
        raw.fixedLengths.resize(fixedBytes);
    if (raw.varLengths.empty())
        raw.varLengths.resize(varCnt);
    assert((int)raw.nullColumns.size() == nullCnt);
    assert((int)raw.fixedLengths.size() == fixedBytes);
    assert((int)raw.varLengths.size() == varCnt);

    const OffsetColumn &off = cols.at(name);
    if (value == nullptr)
        if (off.col.notNull)
            throw NotNullException(name);
        else
            raw.nullColumns[off.nullOffset] = true;
    else
    {
        assert(value->getTypeID() == off.col.typeID);
        if (!off.col.notNull)
            raw.nullColumns[off.nullOffset] = false;
        bool inDomain = off.col.allowedDomain.empty();
        for (auto i = off.col.allowedDomain.begin(); !inDomain && i != off.col.allowedDomain.end(); i++)
            inDomain = (*Type::newFromLiteral(*i, value->getTypeID(), value->getLength()) == *value);
        if (!inDomain)
            throw NotInDomainException(value, name);
        auto bytes = value->toBytes();
        if (value->getFixedLength())
            std::copy(bytes.begin(), bytes.end(), raw.fixedLengths.begin() + off.posOffset);
        else
            raw.varLengths[off.posOffset] = std::move(bytes);
    }
}

