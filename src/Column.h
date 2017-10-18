#ifndef COLUMN_H_
#define COLUMN_H_

#include <vector>
#include <memory>
#include <string>

class Type;
class Value;

/** This struct defines a column structure
 */
struct Column
{
    std::unique_ptr<Type> type; // unique_ptr automatically deallocate when destructed
    std::string name;
    bool notNull;
    std::vector< std::unique_ptr<Value> > allowedDomain; /// Only values in `allowedDomain` is legal
};

#endif // COLUMN_H_
