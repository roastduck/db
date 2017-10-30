#ifndef BASE_TABLE_H_
#define BASE_TABLE_H_

#include <list>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include "Type.h"
#include "Column.h"
#include "Optional.h"
#include "page/ListPage.h"
#include "page/BitmapPage.h"
#include "filesystem/PageCache.h"

/** All the operations on a table
 */
class BaseTable
{
public:
    enum ConDir // Constraint Direction
    {
        EQ, // ==
        NE, // !=
        LT, // <
        LE, // <=
        GT, // >
        GE  // >=
    };

    typedef std::unordered_map<std::string, Column> Cols;

protected:
    struct ConValue
    {
        ConDir dir;
        std::unique_ptr<Type> pivot;
    };

    typedef std::unordered_map< std::string, std::vector<ConValue> > ConsVal;
    typedef std::unordered_map< std::string, std::unique_ptr<Type> > ColVal;

    const Cols cols;

private:
    // Page Identifiers
    short INVALID = 0; // Must be 0, because the page is initialized to be 0
    short RECORD = 1; // Normal data pages
    short PRIMARY = 2; // Primary tree node
    // > 2 for non-clustered index

    PageCache &cache;
    Optional<Cols> primary; // Primary index
    std::vector<Cols> nonClus; // Non-cluster indexes

    std::string dataFile, freeListFile;
    std::vector<ListPage> dataPages; // Keep these metadata of pages in memory reduces overhead
    std::vector<BitmapPage> freeListPages;

    /** Generate columns list according to page identifier
     */
    Cols identToCols(short ident);

    /** Fetch data page #pageID and cache its metadata
     *  NOTE: This page must be already initialized by `newXXXDataPage`
     *  NOTE: The reference may become invalid when call `getDataPage` the next time
     */
    ListPage &getDataPage(int pageID);
    /** Fetch free-list page #pageID and cache its metadata
     *  NOTE: The reference may become invalid when call `getFreeListPage` the next time
     */
    BitmapPage &getFreeListPage(int pageID);

    /** Allocate an unused page and return its Page ID
     */
    int nextFreeDataPage(); /// This function is used by 4 functions below
    int newRecordDataPage();
    int newPrimaryDataPage();
    int newNonClusNodeDataPage(int indexID);
    int newNonClusLeafDataPage(int indexID);
    /** Mark a page back to unused. This will not affect page 0 because it's the entrance
     */
    void destroyDataPage(int pageID);

    /** Check if a record meets ALL the constraints in `cons`, i.e. items in cons are ANDed together
     */
    bool meetCons(ListPage &page, int rank, const ConsVal &cons);

protected:
    void insert(const ColVal &vals);
    void remove(const ConsVal &constraints);
    std::vector<ColVal> select(const std::vector<std::string> &targets, const ConsVal &constraints);

    BaseTable(
        PageCache &_cache, const std::string &_tableName, const Cols &_cols,
        const Optional<Cols> &_primary = None(), const std::vector<Cols> &_nonClus = {}
    );
};

#endif // BASE_TABLE_H_
