#ifndef VALUE_H_
#define VALUE_H_

/** Basic class for variable value
 */
class Value
{
public:
    enum ValueID
    {
        INT,
        FLOAT,
        STRING, /// Here we don't have to distinguish CHAR and VARCHAR
        DATE
    };

    virtual ValueID getValueID() = 0;
};

#endif // VALUE_H_
