#ifndef DATA_PAGE_H_
#define DATA_PAGE_H_

#include <string>
#include <vector>
#include "RawRecord.h"
#include "filesystem/bufmanager/BufPageManager.h"

struct RawRecord;

/** Page containing data records
 *  A record:
 *   | length (int) | null table | fixed-length data | column offset list | variable-length data |
 *   length = 0 means deleted
 *  Tailer:
 *   ... Record offset list in order (ints) | length of record offset list (int)
 */
class DataPage
{
private:
    const int PAGE_BYTES = 8 * 1024;

    BufPageManager bufManager;
    int fileID, pageID, bufIndex, nullColumns, fixedLengths, varLengths;
    unsigned char *bufPtr;

    /** Convert RawData to actual bytes
     */
    std::vector<unsigned char> embed(const RawRecord &rawRecord) const;

    /** Convert bytes to RawData
     */
    RawRecord pack(const unsigned char *bytes) const;

    void prepareBuf() { bufPtr = (unsigned char*)bufManager.getPage(fileID, pageID, bufIndex); }

    int &recordOffset(int rank) { return *(((int*)bufPtr) + (PAGE_BYTES / 4) - rank - 1); }

    int &getSize() { return *(((int*)bufPtr) + (PAGE_BYTES / 4)); }

    /** This describe a situation when body part meets tailer part
     */
    bool certainlyFull();

public:
    /** Construct a new DataPage
     *  You need to pass in the global BufPageManager
     */
    DataPage(BufPageManager &_bugManager, int _fileID, int _pageID, int _nullColumns, int _fixedLengths, int _varLengths);

    /** Add a record into this page
     *  @return : true for success
     */
    bool addRecord(const RawRecord &record);

    /** Iterate through all the record and call the callback function
     *  @param callback : Callback function. bool function(const RawRecord &). It returns false for stop iteration
     */
    void iter(bool(*callback)(const RawRecord&));

    /** Delete a record at given position
     */
    void delRecord(int offset);
};

#endif // DATA_PAGE_H_
