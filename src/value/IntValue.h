#ifndef INT_VALUE_H_
#define INT_VALUE_H_

#include "../Value.h"

/** Integer value
 */
class IntValue : Value
{
private:
    int val;

public:
    int getVal() { return val; }
    void setVal(int _val) { val = _val; }

    ValueID getValueID() override { return ValueID::INT; }
};

#endif // INT_VALUE_H_
