#ifndef FLOAT_H_
#define FLOAT_H_

#include "../Type.h"

/** FLOAT type
 */
class FloatType : Type
{
public:
    TypeID getTypeID() override { return TypeID::FLOAT; }
};

#endif // FLOAT_H_
