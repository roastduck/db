#include "DataPage.h"
#include "filesystem/PageMgr.h"

DataPage::DataPage(
    PageCache &_pageCache, const std::string &_filename, int _pageID,
    int _nullColumns, int _fixedLengths, int _varLengths
)
    : nullColumns(_nullColumns), fixedLengths(_fixedLengths), varLengths(_varLengths),
      mutByte(_pageCache.getPage(_filename, _pageID)), constByte(_pageCache.getConstPage(_filename, _pageID)),
      mutInt(PageCache::MutIntIter(mutByte)), constInt(PageCache::ConstIntIter(constByte))
{}

std::vector<unsigned char> DataPage::embed(const RawRecord &rawRecord) const
{
    std::vector<unsigned char> ret(1); // 1 = preserved for length
    for (int i = 0; i < (int)rawRecord.nullColumns.size(); i++)
    {
        if (i % 8 == 0)
            ret.push_back(0);
        ret.back() |= 1 << i;
    }
    for (unsigned char byte : rawRecord.fixedLengths)
        ret.push_back(byte);
    int offset = ret.size() + rawRecord.varLengths.size() * 4;
    for (const auto &var : rawRecord.varLengths)
    {
        int item = offset += var.size();
        ret.push_back(item & 0x000000ff);
        ret.push_back(item & 0x0000ff00);
        ret.push_back(item & 0x00ff0000);
        ret.push_back(item & 0xff000000);
    }
    for (const auto &var : rawRecord.varLengths)
        for (unsigned char byte : var)
            ret.push_back(byte);
    ret[0] = ret.size();
    return ret;
}

RawRecord DataPage::pack(PageCache::ConstByteIter bytes) const
{
    RawRecord ret;
    int offset = 0;
    for (int i = 0; i < nullColumns; i++)
    {
        if (i % 8 == 0)
            offset++;
        ret.nullColumns.push_back(bytes[offset] & (1 << i));
    }
    for (int i = 0; i < fixedLengths; i++)
        ret.fixedLengths.push_back(bytes[++offset]);
    int varOffset = offset + varLengths;
    ret.varLengths.resize(varLengths);
    for (int i = 0; i < varLengths; i++)
        for (int j = 0; j < bytes[++offset]; j++)
            ret.varLengths[i].push_back(bytes[++varOffset]);
    return ret;
}

bool DataPage::certainlyFull()
{
    int lowestRecordByte = getRecPos(getSize() - 1);
    int highestHeaderByte = 2 + getSize() - 1;
    assert(lowestRecordByte > highestHeaderByte);
    return lowestRecordByte == highestHeaderByte + 1;
}

void DataPage::insert(const std::vector<unsigned char> &bytes, int pos, int rank)
{
    std::copy(bytes.begin(), bytes.end(), mutByte + pos);
    int size = getSize();
    for (int i = rank + 1; i <= size; i++)
        setRecPos(i, getRecPos(i - 1));
    setRecPos(rank, pos);
    setSize(size + 1);
}

int DataPage::addRecord(const RawRecord &record)
{
    if (certainlyFull())
        return -1;
    std::vector<unsigned char> bytes = embed(record);
    int size = getSize();

    int freeEnd = PageMgr::PAGE_SIZE;
    for (int i = 0; i < size; i++)
    {
        int recPos = getRecPos(i);
        int recLength = *PageCache::ConstIntIter(constByte + recPos);
        if (freeEnd - (recPos + recLength) >= int(bytes.size()))
        {
            insert(bytes, recPos + recLength, i);
            return i;
        }
        freeEnd = recPos;
    }
    if (freeEnd - (2 + size) >= int(bytes.size()))
    {
        insert(bytes, 2 + size, size + 1);
        return size + 1;
    }
    return -1;
}

void DataPage::iter(bool(*callback)(int, const RawRecord&))
{
    int size = getSize();
    for (int i = 0; i < size; i++)
    {
        int pos = getRecPos(i);
        bool cont = callback(pos, pack(constByte + pos));
        if (!cont)
            break;
    }
}

void DataPage::delRecord(int offset)
{
    *PageCache::MutIntIter(constByte + offset) = 0; // set length to 0 temporarily
    int size = getSize();
    setSize(size - 1);
    for (int i = 0; i < size; i++)
    {
        int length = *PageCache::ConstIntIter(constByte + getRecPos(i));
        if (!length)
        {
            for (int j = i + 1; j < size; j++)
                setRecPos(j - 1, getRecPos(j));
            break;
        }
    }
}

