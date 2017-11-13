#ifndef FOREIGN_KEY_VIOLATED_EXCEPTION_H_
#define FOREIGN_KEY_VIOLATED_EXCEPTION_H_

#include <cassert>
#include <stdexcept>

class ForeignKeyViolatedException : public std::runtime_error
{
public:
    ForeignKeyViolatedException(
        const std::string &referrer, const std::string &referrerCols,
        const std::string &referee, const std::string &refereeCols
    )
        : std::runtime_error(
            "Foreign key constraint below is violated: " +
            referrer + " (" + referrerCols + ") => " +
            referee + " (" + refereeCols + ")"
        )
    {}
};

#endif // FOREIGN_KEY_VIOLATED_EXCEPTION_H_
