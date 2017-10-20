#ifndef SAME_NAME_EXCEPTION_H_
#define SAME_NAME_EXCEPTION_H_

#include <string>
#include <stdexcept>

/** Two column has the same name
 */
class SameNameException : public std::runtime_error
{
public:
    std::string name;

    SameNameException(const std::string &_name)
        : std::runtime_error("Name \"" + _name + "\" used for more than 1 column"),
          name(_name)
        {}
};

#endif // SAME_NAME_EXCEPTION_H_
