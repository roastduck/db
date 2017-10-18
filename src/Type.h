#ifndef TYPE_H_
#define TYPE_H_

/** Base class for all data types
 */
class Type
{
public:
    enum TypeID
    {
        INT,
        FLOAT,
        DATE,
        CHAR,
        VARCHAR,
    };

    virtual TypeID getTypeID() = 0;
};

#endif // TYPE_H_
