#ifndef NO_SUCH_THING_EXCEPTION_H_
#define NO_SUCH_THING_EXCEPTION_H_

#include <cassert>
#include <stdexcept>
#include "../utils.h"

class NoSuchThingException : public std::runtime_error
{
public:
    NoSuchThingException(const std::string &forWhat, const std::string &name)
        : std::runtime_error("No such " + toLower(forWhat) + " named " + name)
    {}
};

#endif // NO_SUCH_THING_EXCEPTION_H_
