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

private:
    Optional<Index> primary; // Primary index
    std::vector<Index> nonClus; // Non-cluster indexes

    /** Check if a record meets ALL the constraints in `cons`, i.e. items in cons are ANDed together
     */
    bool meetCons(ListPage &page, int rank, const ConsVal &cons);

protected:
    BaseTable
    (
        PageCache &_cache, const std::string &_tableName, const Cols &_cols,
        const Optional<Index> &_primary = None(), const std::vector<Index> &_nonClus = {}
    )
        : TablePages(_cache, _tableName, _cols, _primary, _nonClus),
          primary(_primary), nonClus(_nonClus)
    {}

    void insert(const ColVal &vals);
    void remove(const ConsVal &constraints);
    std::vector<ColVal> select(const std::vector<std::string> &targets, const ConsVal &constraints);
};

#endif // BASE_TABLE_H_
