#ifndef ILLEGAL_FIELD_EXCEPTION_H_
#define ILLEGAL_FIELD_EXCEPTION_H_

#include <stdexcept>

class IllegalFieldException : public std::runtime_error
{
public:
    IllegalFieldException(const std::string &table, const std::string &field)
        : std::runtime_error("Illegal field: " + (table.empty() ? "" : table + ".") + field)
    {}
};

#endif // ILLEGAL_FIELD_EXCEPTION_H_
