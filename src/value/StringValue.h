#ifndef STRING_VALUE_H_
#define STRING_VALUE_H_

#include <string>

#include "../Value.h"

/** String value
 */
class StringValue : Value
{
private:
    std::string val;

public:
    const std::string &getVal() { return val; }
    void setVal(const std::string &_val) { val = _val; }

    ValueID getValueID() override { return ValueID::STRING; }
};

#endif // STRING_VALUE_H_
