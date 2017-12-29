#ifndef TABLE_H_
#define TABLE_H_

#include "BaseTable.h"

/** Literal interface for Table
 */
class Table : public BaseTable
{
public:
    struct ConLiteral
    {
        ConDir dir;
        std::string pivot;
    };

    typedef std::unordered_map< std::string, std::vector<ConLiteral> > ConsL;
    typedef std::unordered_map< std::string, Optional<std::string> > ColL;
    using BaseTable::ColVal;

private:
    /** Convert constraint represented by literals to by values
     */
    ConsVal genConstraints(const ConsL &literals) const;

    /** Convert columns (or record) represented by literals to by values
     */
    ColVal genVals(const ColL &literals) const;

public:
    Table(
        PageCache &_cache, const std::string &_tableName, const Cols &_cols,
        const Optional<Index> &_primary = None(), const std::vector<Index> &_nonClus = {}
    )
        : BaseTable(_cache, _tableName, _cols, _primary, _nonClus)
    {}

    /** Insert a batch of maps of (column name, literal) into the table
     */
    void insert(const std::vector<ColL> &literals);

    /** Insert a map of (column name, literal) into the table
     */
    void insert(const ColL &literals);

    /** Delete records that meet the constraint
     */
    void remove(const ConsL &constraints, const OuterCons &oCons = {})
    {
        BaseTable::remove(genConstraints(constraints), oCons);
    }

    /** Select records
     */
    std::vector<ColVal> select(const std::vector<std::string> &targets, const ConsL &constraints, const OuterCons &oCons = {})
    {
        return BaseTable::select(targets, genConstraints(constraints), oCons);
    }
};

#endif // TABLE_H_
