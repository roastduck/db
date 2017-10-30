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
    typedef std::unordered_map< std::string, std::string > ColL;

private:
    /** Convert constraint represented by literals to by values
     */
    ConsVal genConstraints(const ConsL &literals);

    /** Convert columns (or record) represented by literals to by values
     */
    ColVal genVals(const ColL &literals);

public:
    Table(
        PageCache &_cache, const std::string &_tableName, const Cols &_cols,
        const Optional<Cols> &_primary = None(), const std::vector<Cols> &_nonClus = {}
    )
        : BaseTable(_cache, _tableName, _cols, _primary, _nonClus)
    {}

    /** Insert a map of (column name, literal) into the table
     */
    void insert(const ColL &literals)
    {
        BaseTable::insert(genVals(literals));
    }

    /** Delete records that meet the constraint
     */
    void remove(const ConsL &constraints)
    {
        BaseTable::remove(genConstraints(constraints));
    }

    /** Select records
     */
    std::vector<ColVal> select(const std::vector<std::string> &targets, const ConsL &constraints)
    {
        return BaseTable::select(targets, genConstraints(constraints));
    }
};

#endif // TABLE_H_
