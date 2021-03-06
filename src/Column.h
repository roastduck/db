#ifndef COLUMN_H_
#define COLUMN_H_

#include <vector>
#include <memory>
#include <string>
#include "Type.h"

/** This struct defines a column structure
 */
struct Column
{
    Type::TypeID typeID;
    int length; // Data length for CHAR and VARCHAR. Display length for INT
    bool notNull;
};

#endif // COLUMN_H_
