#ifndef TABLE_STRUCT_H_
#define TABLE_STRUCT_H_

#include <vector>
#include <memory>

#include "Column.h"

struct RawRecord;

/** This class describes the structure of a table
 *  Including:
 *  - What type each column is of
 *  - Indexes connected to this table
 */
class TableStruct
{
public:
    /** Construct a TableStruct with column definitions
     */
    TableStruct(const std::vector<Column> &columns);

    /** Total number of bytes does the fixed-length data occupy per record
     */
    int getFixedLengthSize();

    /** Number of nullable column
     */
    int getNullableCount();

    /** Number of variable-length column
     */
    int getVarLengthCount();

    /** Get a column from a raw record
     *  @param raw : Byte representation of a record
     *  @param name : Column name
     *  @return : The column value. null_ptr for null.
     */
    std::unique_ptr<Value> getVariable(const RawRecord &raw, std::string name);

    /** Set a column of a raw record
     *  @param raw : Byte representation of a record
     *  @param name : Column name
     *  @value : Column value to be set. null_ptr for null.
     */
    void setVariable(RawRecord &raw, std::string name, const std::unique_ptr<Value> &value);
};

#endif // TABLE_STRUCT_H_
