#ifndef BASE_TABLE_H_
#define BASE_TABLE_H_

#include <list>
#include <memory>
#include <vector>
#include "Type.h"
#include "Column.h"
#include "TablePages.h"

/** All the operations on a table
 */
class BaseTable : public TablePages
{
public:
    enum ConDir // Constraint Direction
    {
        // TODO: NULL, NOT_NULL, or treat null as a normal value
        EQ, // ==
        NE, // !=
        LT, // <
        LE, // <=
        GT, // >
        GE  // >=
    };

protected:
    struct ConValue
    {
        ConDir dir;
        std::unique_ptr<Type> pivot;
    };

    typedef std::unordered_map< std::string, std::vector<ConValue> > ConsVal;
    typedef std::unordered_map< std::string, std::unique_ptr<Type> > ColVal;
    typedef std::pair< int, int > Pos; // (pageId, slotID)

private:
    Optional<Index> primary; // Primary index
    std::vector<Index> nonClus; // Non-cluster indexes

    /** Check if a record meets ALL the constraints in `cons`, i.e. items in cons are ANDed together
     */
    bool meetCons(ListPage &page, int rank, const ConsVal &cons);

    /** Compare two batches of columns in lexicological order specified in `order`
     *  @return true for `lhs` < `rhs`
     */
    static bool less(const ColVal &lhs, const ColVal &rhs, const Index &order);

    /** Compare two batches of columns in lexicological order specified in `order`
     *  @return true for `lhs` == `rhs`
     */
    static bool equal(const ColVal &lhs, const ColVal &rhs, const Index &order);

    /** Insert a record into a tree node at offset `off`
     *  @return : If not -1, it means a new child ID should be inserted into the upper layer
     */
    int insertAndSplit(int pageID, const ColVal &vals, int off);

    /** Recursively insert a record into an index tree
     *  @return : If not -1, it means a new child ID should be inserted into the upper layer
     *  @throws NotUniqueException
     */
    int insertRecur(int pageID, const ColVal &vals, const Index &index);

    /** Insert records into the first empty position found in O(n) manner
     *  @return : Page ID inserted in. This Page ID will not change
     */
    int insertLinear(int pageID, const ColVal &vals);

    /** Find in `index` the first position to greater (equal) than `vals`
     */
    Pos findFirst(int pageID, const ColVal &vals, const Index &index, bool equal);

    /** Select records in O(n) manner
     *  It starts scanning at the `start` position, stops when current value > (or >=) `stopV`,
     *  according to `stopIdx` and `stopEq`
     */
    std::vector<ColVal> selectLinear(
        const Index &targets, const ConsVal &constraints, const Pos &start = Pos(0, 0),
        const ColVal &stopV = {}, const Index &stopIdx = {}, bool stopEq = true
    );

    /** When root node has a new neighbour, we have to add a new root for them, but the root (entrance) ID
     *  must be unique. So we have to rotate after that
     */
    void rotateRoot(int rootID, int newChildRID);

protected:
    BaseTable
    (
        PageCache &_cache, const std::string &_tableName, const Cols &_cols,
        const Optional<Index> &_primary = None(), const std::vector<Index> &_nonClus = {}
    );

    void insert(const ColVal &vals);
    void remove(const ConsVal &constraints);
    std::vector<ColVal> select(const Index &targets, const ConsVal &constraints);
};

#endif // BASE_TABLE_H_
