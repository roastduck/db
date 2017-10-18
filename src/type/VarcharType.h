#ifndef VARCHAR_TYPE_H_
#define VARCHAR_TYPE_H_

#include "../Type.h"

class VarcharType : Type
{
public:
    VarcharType(int length);

    TypeID getTypeID() override { return TypeID::VARCHAR; }
};

#endif // VARCHAR_TYPE_H_
