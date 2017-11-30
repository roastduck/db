#ifndef VARCHAR_TYPE_H_
#define VARCHAR_TYPE_H_

#include "CharType.h"

/** Currently this type is completely same as CharType
 */
class VarcharType : public CharType
{
public:
    VarcharType(int _length) : CharType(_length) {}
    TypeID getTypeID() const override { return VARCHAR; }
};

#endif // VARCHAR_TYPE_H_
