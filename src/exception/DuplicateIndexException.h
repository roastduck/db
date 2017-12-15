#ifndef DUPLICATE_INDEX_EXCEPTION_H_
#define DUPLICATE_INDEX_EXCEPTION_H_

#include <string>
#include <stdexcept>

class DuplicateIndexException : public std::runtime_error
{
public:
    DuplicateIndexException(const std::string &tbName, const std::string &index)
        : std::runtime_error("Index " + tbName + "(" + index + ") already exists")
    {}
};

#endif // DUPLICATE_INDEX_EXCEPTION_H_
