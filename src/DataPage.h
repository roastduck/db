#ifndef DATA_PAGE_H_
#define DATA_PAGE_H_

#include <string>

struct RawRecord;

/** Page containing data records
 */
class DataPage
{
public:
    /** Construct a new DataPage
     *  @param fileName : Load data page from the file. If not exists, create one
     *  @param pageID : Load data from this page. If not exists, create one
     *  @param fixedLegthSize : Bytes that fixed-length data used in total per record
     *  @param nullableCount : Number of nullable column
     *  @param varLengthCount : Number of variable-length column
     */
    DataPage(std::string fileName, int pageID, int fixedLengthSize, int nullableCount, int varLengthCount);

    /** Get total number of record already stored in this page
     */
    int getSize();

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
