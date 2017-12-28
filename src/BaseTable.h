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
        IS_NULL,
        IS_NOT_NULL,
        EQ, // ==
        NE, // !=
        LT, // <
        LE, // <=
        GT, // >
        GE  // >=
    };

    struct OuterCon
    {
        ConDir dir;
        std::string col1, col2;
    };
    typedef std::vector<OuterCon> OuterCons;

protected:
    struct ConValue
    {
        ConDir dir;
        std::unique_ptr<Type> pivot;
    };

    typedef std::unordered_map< std::string, std::vector<ConValue> > ConsVal;
    typedef std::unordered_map< std::string, std::unique_ptr<Type> > ColVal;
    typedef std::pair< int, int > Pos; // (pageId, slotID)
    typedef std::pair< std::pair<ColVal, bool /* open interval */>, std::pair<ColVal, bool> > Bound;

private:
    const int ENTRY_PAGE = 0;

    Index allColumns;
    Optional<Index> primary; // Primary index
    std::vector<Index> nonClus; // Non-cluster indexes

    /** Primary entry page ID
     */
    int priEntry();
    void setPriEntry(int e);

    /** Entry page ID of a non-cluster index
     */
    int ncEntry(int indexID);
    void setNcEntry(int indexID, int e);
    void delNcEntry(int indexID);

    /** Check if a record meets ALL the constraints in `cons`, i.e. items in cons are ANDed together
     */
    bool meetCons(ListPage &page, int rank, const ConsVal &cons, const OuterCons &oCons) const;

    /** Compare two batches of columns in lexicological order specified in `order`
     *  This function treats null as negative infinity
     *  @return true for `lhs` < `rhs`
     */
    static bool less(const ColVal &lhs, const ColVal &rhs, const Index &order);

    /** Compare two batches of columns in lexicological order specified in `order`
     *  This function treats null as negative infinity
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
    int insertLinear(int pageID, const ColVal &vals, short ident);

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
     *  The header node will not be touched
     *  @param onlyOne : return after removing any record
     *  @return true if there is no records left
     */
    bool removeLinear(int pageID, const ConsVal &constraints, const OuterCons &oCons, bool onlyOne);

    /** Find in `index` the first position to greater (equal) than `vals`
     *  @return might be (-1, 0)
     */
    Pos findFirst(int pageID, const ColVal &vals, const Index &index, bool equal);

    void addToSelection(
        std::vector<ColVal> &ret, int pageID, int off, const Index &targets, const ConsVal &constraints, const OuterCons &oCons
    );

    /** Select records in O(n) manner
     *  It starts scanning at the `start` position, stops when current value > (or >=) `stopV`,
     *  according to `stopIdx` and `stopEq`
     *  @param `ret` is output parameter, answer will be APPENDED to this
     */
    void selectLinear(
        std::vector<ColVal> &ret,
        const Index &targets, const ConsVal &constraints, const OuterCons &oCons, const Pos &start = Pos(0, 0),
        const ColVal &stopV = {}, const Index &stopIdx = {}, bool stopEq = true
    );

    void selectRefLinear(
        std::vector<ColVal> &ret,
        const Index &targets, const ConsVal &constraints, const OuterCons &oCons, const Pos &start,
        const ColVal &stopV, const Index &stopIdx, bool stopEq
    );

    /** When root node has a new neighbour, we have to add a new root for them
     */
    void rotateRoot(int newChildLID, int newChildRID, const Index &index, short ident);

    /** Same as above, when removing a root, we have to register the new root
     */
    void removeRoot(int rootID);

    void updLBound(ColVal &l, const std::string &name, bool &open, const std::unique_ptr<Type> &_l, bool _open);
    void updRBound(ColVal &r, const std::string &name, bool &open, const std::unique_ptr<Type> &_r, bool _open);
    Bound getBound(const ConsVal &constraints, const Index &index);

    void delIndexRecur(int pageID);

protected:
    BaseTable
    (
        PageCache &_cache, const std::string &_tableName, const Cols &_cols,
        const Optional<Index> &_primary = None(), const std::vector<Index> &_nonClus = {}
    );

    void insert(const ColVal &vals);
    void remove(const ConsVal &constraints, const OuterCons &oCons);
    std::vector<ColVal> select(const Index &targets, const ConsVal &constraints, const OuterCons &oCons);

public:
    const Optional<Index> &getPrimary() const { return primary; }
    const std::vector<Index> &getNonClus() const { return nonClus; }
    const Index &getAllColumns() const { return allColumns; }

    /** Add a non-cluster index
     *  @return : new index ID
     */
    int addIndex(const Index &index);

    /** Delete a non-cluster index
     */
    void delIndex(int indexID);
};

#endif // BASE_TABLE_H_
