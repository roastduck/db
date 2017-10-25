#ifndef DATA_PAGE_H_
#define DATA_PAGE_H_

#include <string>
#include <vector>
#include <cassert>
#include "RawRecord.h"
#include "filesystem/PageCache.h"

struct RawRecord;

/** Page containing data records
 *  Header:
 *   | next page ID (int) | size i.e. record # (short) | list of record positions in byte from high to low (shorts) ...
 *  A record:
 *   | length (short) | null table | fixed-length data | column length list (shorts) | variable-length data |
 */
class DataPage
{
private:
    const int nullColumns, fixedLengths, varLengths;
    PageCache::MutByteIter mutByte;
    PageCache::ConstByteIter constByte;
    PageCache::MutShortIter mutShort;
    PageCache::ConstShortIter constShort;
    PageCache::MutIntIter mutInt;
    PageCache::ConstIntIter constInt;

    /** Convert RawData to actual bytes
     */
    std::vector<unsigned char> embed(const RawRecord &rawRecord) const;

    /** Convert bytes to RawData
     */
    RawRecord pack(PageCache::ConstByteIter bytes) const;

    /** Insert a record into the body and update the header
     */
    void insert(const std::vector<unsigned char> &bytes, int pos, int rank);

public:
    /** Construct a new DataPage
     *  You need to pass in the global BufPageManager
     */
    DataPage(
        PageCache &_pageCache, const std::string &_filename, int _pageID,
        int _nullColumns, int _fixedLengths, int _varLengths
    );

    /** PageID of next page */
    int getNextPage() { return constInt[0]; }
    void setNextPage(int v) { mutInt[0] = v; }

    /** Number of record in this page */
    short getSize() { return constShort[2]; }
    void setSize(short v) { mutShort[2] = v; }

    /** Position (bytes) of record `rank`
     *  @param rank : start from 0
     */
    short getRecPos(int rank) { return constShort[rank + 3]; }
    void setRecPos(int rank, short v) { mutShort[rank + 3] = v; }

    /** Add a record into this page
     *  @return : index when successful, -1 otherwise
     */
    int addRecord(const RawRecord &record);

    /** Return a record at specific offset
     */
    RawRecord readRecord(int offset) { return pack(mutByte + getRecPos(offset)); }

    /** Delete a record at given index (offset)
     */
    void delRecord(int offset);
};

#endif // DATA_PAGE_H_
