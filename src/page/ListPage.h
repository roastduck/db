#ifndef LIST_PAGE_H_
#define LIST_PAGE_H_

#include <unordered_map>
#include "../Column.h"
#include "../BasePage.h"
#include "../exception/NoSuchThingException.h"

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

    PageCache::ConstByteIter accessConst(int rank) { return constByte + HEADER_SIZE + rank * recBytes; }
    PageCache::MutByteIter accessMut(int rank) { return mutByte + HEADER_SIZE + rank * recBytes; }

    /** i-th item in null map
     */
    bool getNull(int rank, int i)
    {
        return (accessConst(rank)[i >> 3] >> (i & 7)) & 1;
    }
    void setNull(int rank, int i, bool v)
    {
        unsigned char &ref = accessMut(rank)[i >> 3];
        ref = ((ref & ~(1 << (i & 7))) | (v << (i & 7)));
    }

public:
    static const int HEADER_SIZE = 12;

    /** Construct with column definitions
     */
    ListPage(
        PageCache &_pageCache, const std::string &_filename, int _pageID,
        const std::unordered_map<std::string, Column> &_cols
    )
        : BasePage(_pageCache, _filename, _pageID)
    {
        setCols(_cols);
    }

    /** Construct without column set. But it has to be set using `setCols`
     */
    ListPage(PageCache &_pageCache, const std::string &_filename, int _pageID)
        : BasePage(_pageCache, _filename, _pageID)
    {}

    /** Set column list for every record
     */
    void setCols(const std::unordered_map<std::string, Column> &_cols);

    /** This marks different usage of ListPage
     */
    short getIdent() { return constShort[0]; }
    void setIdent(short v) { mutShort[0] = v; }

    /** Max record number
     */
    int getMaxSize() const { return (PageMgr::PAGE_SIZE - HEADER_SIZE) / recBytes; }

    /** How many records stored in this page
     */
    short getSize() { return constShort[1]; }
    void setSize(short v) { assert(v >= 0); mutShort[1] = v; }

    /** ID of the previous page
     *  @return -1 for null
     */
    int getPrev() { return constInt[1] - 1; }
    void setPrev(int v) { mutInt[1] = v + 1; }

    /** ID of the next page
     *  @return -1 for null
     */
    int getNext() { return constInt[2] - 1; }
    void setNext(int v) { mutInt[2] = v + 1; }

    /** Get a column from record #rank
     *  @param rank : Record position
     *  @param name : Column name
     *  @return : The column value. null_ptr for null.
     */
    std::unique_ptr<Type> getValue(int rank, const std::string &name);

    std::unordered_map< std::string, std::unique_ptr<Type> > getValues(int rank, const std::vector<std::string> &names);

    /** Set a column to record #rank
     *  @param rank : Record position
     *  @param name : Column name
     *  @param value : Column value to be set. null_ptr for null.
     *  @throw NotNullException
     */
    void setValue(int rank, const std::string &name, const std::unique_ptr<Type> &value);

    void setValues(int rank, const std::unordered_map< std::string, std::unique_ptr<Type> > &values);

    /** Copy a record from rank1 to rank2
     */
    void copy(int rank1, int rank2);

    /** Copy a record across two pages
     *  We CANNOT copy data across pages if there is no primary index, or non-clustered indexes would fail
     */
    static void copy(ListPage *page1, int rank1, ListPage *page2, int rank2);

    /** Copy everything except identifier from one page to another
     */
    static void copy(ListPage *page1, ListPage *page2);
};

#endif // LIST_PAGE_H_
