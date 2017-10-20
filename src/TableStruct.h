#ifndef TABLE_STRUCT_H_
#define TABLE_STRUCT_H_

#include <vector>
#include <memory>
#include <unordered_map>

#include "Column.h"

struct RawRecord;

/** This class describes the structure of a table
 *  Including:
 *  - What type each column is of
 *  - Indexes connected to this table
 */
class TableStruct
{
private:
    struct OffsetColumn
    {
        const Column &col;
        int posOffset; // For fixed: bytes offset. For variable: variable column #
        int nullOffset; // Offset in nullable list
        OffsetColumn(const Column &_col, int _posOffset, int _nullOffset)
            : col(_col), posOffset(_posOffset), nullOffset(_nullOffset) {}
    };

    int fixedBytes, nullCnt, varCnt;
    std::unordered_map<std::string, OffsetColumn> cols;

public:
    /** Construct a TableStruct with column definitions
     *  @throw SameNameException
     */
    TableStruct(const std::vector<Column> &_cols);

    /** Total number of bytes does the fixed-length data occupy per record
     */
    int getFixedLengthSize() const { return fixedBytes; }

    /** Number of nullable column
     */
    int getNullableCount() const { return nullCnt; }

    /** Number of variable-length column
     */
    int getVarLengthCount() const { return varCnt; }

    /** Get a column from a raw record
     *  @param raw : Byte representation of a record
     *  @param name : Column name
     *  @return : The column value. null_ptr for null.
     */
    std::unique_ptr<Type> getVariable(const RawRecord &raw, const std::string &name) const;

    /** Set a column of a raw record
     *  @param raw : Byte representation of a record
     *  @param name : Column name
     *  @value : Column value to be set. null_ptr for null.
     *  @throw NotNullException
     *  @throw NotInDomainException
     */
    void setVariable(RawRecord &raw, const std::string &name, const std::unique_ptr<Type> &value) const;
};

#endif // TABLE_STRUCT_H_
