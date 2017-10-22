#include "DataPage.h"

DataPage::DataPage(BufPageManager &_bugManager, int _fileID, int _pageID, int _nullColumns, int _fixedLengths, int _varLengths)
    : bufManager(_bugManager), fileID(_fileID), pageID(_pageID),
      nullColumns(_nullColumns), fixedLengths(_fixedLengths), varLengths(_varLengths)
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

RawRecord DataPage::pack(const unsigned char *bytes) const
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
    int itemCnt = getSize();
    int lastItemOffset = recordOffset(itemCnt - 1);
    int lastItemLen = *((int*)(bufPtr + lastItemOffset));
    return lastItemOffset + lastItemLen + (itemCnt + 1) * 4 == PAGE_BYTES;
}

int DataPage::addRecord(const RawRecord &record)
{
    prepareBuf();
    if (certainlyFull())
        return -1;
    std::vector<unsigned char> bytes = embed(record);
    int itemCnt = getSize();
    for (int i = 0; i < itemCnt; i++)
    {
        int itemLength = *((int*)(bufPtr + recordOffset(i)));
        int freeBegin = recordOffset(i) + itemLength;
        int freeEnd = i < itemCnt - 1 ?
            recordOffset(i + 1) :
            PAGE_BYTES - (itemCnt + 2) * 4 - 4 /* this another 4 preserved for increasing offset list */;
        if (freeEnd - freeBegin < int(bytes.size()))
            continue;
        std::copy(bytes.begin(), bytes.end(), bufPtr + freeBegin);
        if (itemLength) // itemLength = 0 means original item was deleted
        {
            getSize()++;
            for (int j = i + 1; j < itemCnt; j++)
                recordOffset(j + 1) = recordOffset(j);
            recordOffset(i) = freeBegin;
        }
        return i;
    }
    return -1;
}

void DataPage::iter(bool(*callback)(int, const RawRecord&))
{
    for (int i = 0; i < getSize(); i++)
    {
        bool cont = callback(recordOffset(i), pack(bufPtr + recordOffset(i)));
        if (!cont)
            break;
    }
}

void DataPage::delRecord(int offset)
{
    *((int*)(bufPtr + offset)) = 0;
}

