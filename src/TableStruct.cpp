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
        OffsetColumn off(
            col,
            type->getFixedLength() ? fixedBytes += type->getFixedLength() : varCnt++,
            off.nullOffset = col.notNull ? -1 : nullCnt++
        );
        bool success = cols.insert(std::make_pair(col.name, std::move(off))).second;
        if (!success)
            throw SameNameException(col.name);
    }
}

std::unique_ptr<Type> TableStruct::getVariable(const RawRecord &raw, const std::string &name) const
{
    const OffsetColumn &off = cols.at(name);
    std::unique_ptr<Type> ret = Type::newType(off.col.typeID, off.col.length);
    if (!off.col.notNull && raw.nullColumns[off.nullOffset])
        return nullptr;
    if (ret->getFixedLength())
        ret->fromBytes(std::vector<unsigned char>(
            raw.fixedLengths.begin() + off.posOffset,
            raw.fixedLengths.begin() + off.posOffset + ret->getFixedLength()
        ));
    else
        ret->fromBytes(raw.varLengths[off.posOffset]);
    return ret;
}

void TableStruct::setVariable(RawRecord &raw, const std::string &name, const std::unique_ptr<Type> &value) const
{
    const OffsetColumn &off = cols.at(name);
    assert(value->getTypeID() == off.col.typeID);
    if (value == nullptr)
        if (off.col.notNull)
            throw NotNullException(name);
        else
            raw.nullColumns[off.nullOffset] = true;
    else
    {
        raw.nullColumns[off.nullOffset] = false;
        bool inDomain = off.col.allowedDomain.empty();
        for (auto i = off.col.allowedDomain.begin(); !inDomain && i != off.col.allowedDomain.end(); i++)
            inDomain = (**i == *value);
        if (!inDomain)
            throw NotInDomainException(value, name);
        auto bytes = value->toBytes();
        if (value->getFixedLength())
            std::copy(bytes.begin(), bytes.end(), raw.fixedLengths.begin() + off.posOffset);
        else
            raw.varLengths[off.posOffset] = std::move(bytes);
    }
}

