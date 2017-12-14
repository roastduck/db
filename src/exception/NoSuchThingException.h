#ifndef NO_SUCH_THING_EXCEPTION_H_
#define NO_SUCH_THING_EXCEPTION_H_

#include <cassert>
#include <algorithm>
#include <stdexcept>

class NoSuchThingException : public std::runtime_error
{
private:
    std::string toLower(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), tolower);
        return s;
    }

public:
    NoSuchThingException(const std::string &forWhat, const std::string &name)
        : std::runtime_error("No such " + toLower(forWhat) + " named " + name)
    {}
};

#endif // NO_SUCH_THING_EXCEPTION_H_
