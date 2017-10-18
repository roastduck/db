#ifndef INT_TYPE_H_
#define INT_TYPE_H_

#include "../Type.h"

/** INT type
 */
class IntType : Type
{
public:
    TypeID getTypeID() override { return TypeID::INT; }
};

#endif // INT_TYPE_H_
