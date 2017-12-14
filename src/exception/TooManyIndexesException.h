#ifndef TOO_MANY_INDEXES_EXCEPTION_H_
#define TOO_MANY_INDEXES_EXCEPTION_H_

#include <string>
#include <stdexcept>
#include "../config.h"

class TooManyIndexesException : public std::runtime_error
{
public:
    TooManyIndexesException()
        : std::runtime_error("There can't be more indexes than " + std::to_string(MAX_INDEX_NUM))
    {}
};

#endif // TOO_MANY_INDEXES_EXCEPTION_H_
