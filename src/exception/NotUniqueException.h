#ifndef NOT_UNIQUE_EXCEPTION_H_
#define NOT_UNIQUE_EXCEPTION_H_

#include <string>
#include <vector>
#include <stdexcept>

class NotUniqueException : public std::runtime_error
{
private:
    std::string toString(const std::vector<std::string> &vec)
    {
        std::string ret;
        for (const auto &item : vec)
        {
            if (!ret.empty())
                ret += ",";
            ret += item;
        }
        return ret;
    }

public:
    std::vector<std::string> cols;

    NotUniqueException(const std::vector<std::string> &_cols)
        : std::runtime_error("Column(s) (" + toString(_cols) + ") should be unique"),
          cols(_cols)
        {}
};

#endif // NOT_UNIQUE_EXCEPTION_H_
