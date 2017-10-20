#ifndef TYPE_H_
#define TYPE_H_

#include <string>
#include <memory>
#include <vector>

class Value;

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

    virtual TypeID getTypeID() const = 0;

    /** Return the length (bytes #) if it is a fixed-length type. Otherwise return 0
     */
    virtual int getFixedLength() const = 0;

    /** Save to bytes
     */
    virtual std::vector<unsigned char> toBytes() const = 0;

    /** Load from bytes
     */
    virtual void fromBytes(const std::vector<unsigned char> &bytes) = 0;

    /** Print to string
     */
    virtual std::string toString() const = 0;

    /** Load from string (SQL literal are virtually strings)
     *  @throw InvalidLiteralException
     */
    virtual void fromString(const std::string &s) = 0;

    /** Construct a null *Type object from TypeID
     *  This is used to get metadata from TypeID
     */
    static std::unique_ptr<Type> newType(TypeID typeID, int length);
};

bool operator==(const Type &t1, const Type &t2);
bool operator!=(const Type &t1, const Type &t2);

#endif // TYPE_H_
