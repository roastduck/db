#ifndef REFEREE_NOT_PRIMARY_EXCEPTION_H_
#define REFEREE_NOT_PRIMARY_EXCEPTION_H_

#include <cassert>
#include <stdexcept>

class RefereeNotPrimaryException : public std::runtime_error
{
public:
    RefereeNotPrimaryException(const std::string &refereeCols, const std::string &referee, const std::string &primaryCols)
        : std::runtime_error(
            "Referee columns of a foreign key (" + refereeCols + ") does not match with table " +
            referee + "'s primary key (" + primaryCols + ")"
        )
    {}
};

#endif // REFEREE_NOT_PRIMARY_EXCEPTION_H_
