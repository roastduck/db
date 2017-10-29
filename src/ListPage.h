#ifndef LIST_PAGE_H_
#define LIST_PAGE_H_

#include <unordered_map>
#include "Column.h"
#include "BasePage.h"

/** Page storing a list of records
 *  Header:
 *   identifier (short) | size (short) | prev(int) | next(int)
 */
class ListPage : public BasePage
{
private:
    struct OffsetColumn
    {
        Column col;
        int posOffset; // bytes offset
        int nullOffset; // Offset in nullable list
    };

    int nullCnt, nullBytes, fixedBytes, recBytes;
    std::unordered_map<std::string, OffsetColumn> cols;

    static const int HEADER_SIZE = 12;

    /** This marks different usage of ListPage
     */
    short getIdent() { return constShort[0]; }
    void setIdent(short v) { mutShort[0] = v; }

    PageCache::ConstByteIter accessConst(int rank) { return constByte + HEADER_SIZE + rank * recBytes; }
    PageCache::MutByteIter accessMut(int rank) { return mutByte + HEADER_SIZE + rank * recBytes; }

    /** i-th item in null map
     */
    bool getNull(int rank, int i)
    {
        return (accessConst(rank)[i / 8] >> (i % 8)) & 1;
    }
    void setNull(int rank, int i, bool v)
    {
        unsigned char &ref = accessMut(rank)[i / 8];
        ref = ((ref & ~(1 << (i % 8))) | (v << (i % 8)));
    }

public:
    /** Construct with column definitions
     *  @throw SameNameException
     */
    ListPage(PageCache &_pageCache, const std::string &_filename, int _pageID, const std::vector<Column> &_cols);

    /** How many records stored in this page
     */
    short getSize() { return constShort[1]; }
    void setSize(short v) { mutShort[1] = v; }

    /** ID of the previous page
     */
    int getPrev() { return constInt[1]; }
    void setPrev(int v) { mutInt[1] = v; }

    /** ID of the next page
     */
    int getNext() { return constInt[2]; }
    void setNext(int v) { mutInt[2] = v; }

    /** Get a column from record #rank
     *  @param rank : Record position
     *  @param name : Column name
     *  @return : The column value. null_ptr for null.
     */
    std::unique_ptr<Type> getValue(int rank, const std::string &name);

    /** Set a column to record #rank
     *  @param rank : Record position
     *  @param name : Column name
     *  @param value : Column value to be set. null_ptr for null.
     *  @throw NotNullException
     *  @throw NotInDomainException
     */
    void setValue(int rank, const std::string &name, const std::unique_ptr<Type> &value);

    /** Copy a record from rank1 to rank2
     */
    void copy(int rank1, int rank2);
};

#endif // LIST_PAGE_H_
