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
    typedef std::pair< std::pair<ColVal, bool>, std::pair<ColVal, bool> > Bound;

private:
    Index allColumns;
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

    /** Update node from its children
     */
    void updNode(int pageID, int offset, const Index &index);

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

    /**
     *  @return : If returns x, parent[parentOffset + x] should be remvoed, too
     */
    Optional<int> removeAndMerge(int pageID, int off);

    /** Recursively delete a record from an index tree
     *  NOTE: If there are more than one record in a non-clustered index match the constraint,
     *        only one will be deleted
     *  @return : Same as `removeAndMerge`
     */
    Optional<int> removeRecur(int pageID, const ColVal &vals, const Index &index);

    /** Remove record(s) in a O(n) manner
     *  @param onlyOne : return after removing any record
     *  @return true if there is no records left
     */
    bool removeLinear(int pageID, const ConsVal &constraints, bool onlyOne);

    /** Find in `index` the first position to greater (equal) than `vals`
     */
    Pos findFirst(int pageID, const ColVal &vals, const Index &index, bool equal);

    void addToSelection(std::vector<ColVal> &ret, int pageID, int off, const Index &targets, const ConsVal &constraints);

    /** Select records in O(n) manner
     *  It starts scanning at the `start` position, stops when current value > (or >=) `stopV`,
     *  according to `stopIdx` and `stopEq`
     *  @param `ret` is output parameter, answer will be APPENDED to this
     */
    void selectLinear(
        std::vector<ColVal> &ret,
        const Index &targets, const ConsVal &constraints, const Pos &start = Pos(0, 0),
        const ColVal &stopV = {}, const Index &stopIdx = {}, bool stopEq = true
    );

    void selectRefLinear(
        std::vector<ColVal> &ret,
        const Index &targets, const ConsVal &constraints, const Pos &start,
        const ColVal &stopV, const Index &stopIdx, bool stopEq
    );

    /** When root node has a new neighbour, we have to add a new root for them, but the root (entrance) ID
     *  must be unique. So we have to rotate after that
     */
    void rotateRoot(int rootID, int newChildRID, const Index &index, short ident);

    /** Same as above, when removing a root, we are actually moving its content to its only child and remove
     *  the child
     */
    void removeRoot(int rootID);

    Bound getBound(const ConsVal &constraints, const Index &index);

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
