#ifndef RECORD_TOO_LARGE_EXCEPTION_H_
#define RECORD_TOO_LARGE_EXCEPTION_H_

#include <string>
#include <stdexcept>
#include "../config.h"

class RecordTooLargeException : public std::runtime_error
{
public:
    RecordTooLargeException(int bytes) :
        std::runtime_error(
            "The size of each record (" + std::to_string(bytes) + " bytes) is too large. "
            "The maximum is " + std::to_string(MAX_RECORD_BYTES) + " bytes."
        )
    {}
};

#endif // RECORD_TOO_LARGE_EXCEPTION_H_
