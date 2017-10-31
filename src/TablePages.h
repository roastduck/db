#ifndef TABLE_PAGES_H_
#define TABLE_PAGES_H_

#include <string>
#include <unordered_map>
#include "Optional.h"
#include "page/ListPage.h"
#include "page/BitmapPage.h"
#include "filesystem/PageCache.h"

/** Manages pages used in a Table
 */
class TablePages
{
public:
    typedef std::unordered_map<std::string, Column> Cols;
    typedef std::vector< std::string > Index;

protected:
    // Page Identifiers
    static const short INVALID = 0; // Must be 0, because the page is initialized to be 0
    static const short RECORD = 1; // Normal data pages
    static const short REF = 2; // Leaf of non-clustered index
    static const short PRIMARY = 3; // Primary tree node
    // > 3 for non-clustered index

private:
    PageCache &cache;

protected:
    const Cols recCols;

private:
    Cols refCols, priCols;
    std::vector<Cols> nonClusCols; // (Node, Leaf)

    std::string dataFile, freeListFile;
    std::vector<ListPage> dataPages; // Keep these metadata of pages in memory reduces overhead
    std::vector<BitmapPage> freeListPages;

    Cols identToCols(short ident);

    int nextFreeDataPage();

protected:
    TablePages(
        PageCache &_cache, const std::string &_tableName, const Cols &_cols,
        const Optional<Index> &_primary = None(), const std::vector<Index> &_nonClus = {}
    );

    /** Fetch free-list page #pageID and cache its metadata
     *  NOTE: The reference may become invalid when call `getFreeListPage` the next time
     */
    BitmapPage &getFreeListPage(int pageID);

    /** Fetch data page #pageID and cache its metadata
     *  NOTE: This page must be already initialized by `newXXXDataPage`
     *  NOTE: The reference may become invalid when call `getDataPage` the next time
     */
    ListPage &getDataPage(int pageID);

    /** Allocate an unused page and return its Page ID
     */
    int newRecordDataPage();
    int newRefDataPage();
    int newPrimaryDataPage();
    int newNonClusDataPage(int indexID);

    /** Mark a page back to unused. This will not affect page 0 because it's the entrance
     */
    void destroyDataPage(int pageID);
};

#endif // TABLE_PAGES_H_
