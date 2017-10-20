#ifndef NOT_IN_DOMAIN_EXCEPTION_H_
#define NOT_IN_DOMAIN_EXCEPTION_H_

#include <string>
#include <stdexcept>
#include "../Type.h"

/** Trying to set value not in user defined domain */
class NotInDomainException : public std::runtime_error
{
public:
    // May store wrong value here, but requiring implementing generic copy of Type

    std::string colName;

    NotInDomainException(const std::unique_ptr<Type> &val, const std::string &_colName)
        : std::runtime_error("Value \"" + val->toString() + "\" not in user defined domain of column \"" + _colName + "\""),
          colName(_colName)
        {}
};

#endif // NOT_IN_DOMAIN_EXCEPTION_H_
