#ifndef NOT_NULL_EXCEPTION_H_
#define NOT_NULL_EXCEPTION_H_

#include <string>
#include <stdexcept>

/** Trying to set a column marked as NOT NULL to NULL
 */
class NotNullException : public std::runtime_error
{
public:
    std::string colName;

    NotNullException(const std::string &_colName)
        : std::runtime_error("Column \"" + _colName + "\" cannot be NULL"),
          colName(_colName)
        {}
};

#endif // NOT_NULL_EXCEPTION_H_
