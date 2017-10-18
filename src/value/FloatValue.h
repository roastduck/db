#ifndef FLOAT_VALUE_H_
#define FLOAT_VALUE_H_

#include "../Value.h"

/** Floating-point number value
 */
class FloatValue : Value
{
private:
    double val; // Yes, we use 64-bit floating number

public:
    double getVal() { return val; }
    void setVal(double _val) { val = _val; }

    ValueID getValueID() override { return ValueID::FLOAT; }
};

#endif // FLOAT_VALUE_H_
