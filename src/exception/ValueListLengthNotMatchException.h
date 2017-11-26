#ifndef VALUE_LIST_LENGTH_NOT_MATCH_EXCEPTION_H_
#define VALUE_LIST_LENGTH_NOT_MATCH_EXCEPTION_H_

#include <cassert>
#include <stdexcept>

class ValueListLengthNotMatchException : public std::runtime_error
{
public:
    ValueListLengthNotMatchException(int actual, int expect)
        : std::runtime_error(
            "Wrong length of the value list: " + std::to_string(actual) +
            ", " + std::to_string(expect) + " expected"
        )
    {}
};

#endif // VALUE_LIST_LENGTH_NOT_MATCH_EXCEPTION_H_
