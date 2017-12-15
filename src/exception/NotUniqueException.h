#ifndef NOT_UNIQUE_EXCEPTION_H_
#define NOT_UNIQUE_EXCEPTION_H_

#include <string>
#include <vector>
#include <stdexcept>
#include "../utils.h"

class NotUniqueException : public std::runtime_error
{
public:
    std::vector<std::string> cols;

    NotUniqueException(const std::vector<std::string> &_cols)
        : std::runtime_error("Column(s) (" + commaJoin(_cols) + ") should be unique"),
          cols(_cols)
        {}
};

#endif // NOT_UNIQUE_EXCEPTION_H_
