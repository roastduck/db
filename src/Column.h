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
    Type::TypeID typeID; // unique_ptr automatically deallocate when destructed
    int length; // Currently only for CHAR
    std::string name;
    bool notNull;
    std::vector<std::string> allowedDomain; /// Only literals in `allowedDomain` is legal
};

#endif // COLUMN_H_
