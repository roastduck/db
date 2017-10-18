#ifndef DATE_TYPE_H_
#define DATE_TYPE_H_

#include "../Type.h"

class DateType : Type
{
public:
    TypeID getTypeID() override { return TypeID::DATE; }
};

#endif // DATE_TYPE_H_
