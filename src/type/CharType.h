#ifndef CHAR_TYPE_H_
#define CHAR_TYPE_H_

#include "../Type.h"

/** CHAR type
 *  A CHAR is a fixed-length string
 */
class CharType : Type
{
public:
    CharType(int length);

    TypeID getTypeID() override { return TypeID::CHAR; }
};

#endif // CHAR_TYPE_H_
