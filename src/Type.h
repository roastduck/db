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
protected:
    const int length;

public:
    Type(int _length = 0) : length(_length) {}

    enum TypeID
    {
        INT,
        FLOAT,
        DATE,
        CHAR
        // VARCHAR is treated as CHAR
    };

    virtual TypeID getTypeID() const = 0;

    /** Return the length (bytes #)
     */
    virtual int getFixedLength() const = 0;

    /** Orignal length set by user
     */
    int getLength() const { return length; }

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
    static std::unique_ptr<Type> newType(TypeID typeID, int length = 0);

    /** Make a new object from literal
     */
    static std::unique_ptr<Type> newFromLiteral(const std::string &literal, TypeID typeID, int length = 0);

    /** Make a new object from bytes representation
     */
    static std::unique_ptr<Type> newFromBytes(const std::vector<unsigned char> &bytes, TypeID typeID, int length = 0);
};

bool operator==(const Type &t1, const Type &t2);
bool operator!=(const Type &t1, const Type &t2);

#endif // TYPE_H_
