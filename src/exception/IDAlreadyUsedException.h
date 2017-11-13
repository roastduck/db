#ifndef ID_ALREADY_USED_EXCEPTION_H_
#define ID_ALREADY_USED_EXCEPTION_H_

#include <cassert>
#include <stdexcept>

class IDAlreadyUsedException : public std::runtime_error
{
public:
    IDAlreadyUsedException(const std::string &forWhat, const std::string &name)
        : std::runtime_error("Name " + name + " for " + forWhat + " already in use")
    {}
};

#endif // ID_ALREADY_USED_EXCEPTION_H_
