#ifndef TOO_MANY_FIELDS_EXCEPTION_H_
#define TOO_MANY_FIELDS_EXCEPTION_H_

#include <string>
#include <stdexcept>
#include "../config.h"

class TooManyFieldsException : public std::runtime_error
{
public:
    TooManyFieldsException(int num)
        : std::runtime_error(
            "There are too many fields in the table (" + std::to_string(num) +
            " fields). The maximum is " + std::to_string(MAX_COLUMN_NUM)
        )
    {}
};

#endif // TOO_MANY_FIELDS_EXCEPTION_H_

