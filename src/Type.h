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
private:
    bool null;

public:
    enum TypeID
    {
        INT,
        FLOAT,
        DATE,
        CHAR,
        VARCHAR,
    };

    Type() : null(true) {}

    bool isNull() { return null; }

    virtual TypeID getTypeID() = 0;

    /** Return the length (bytes #) if it is a fixed-length type. Otherwise return 0
     */
    virtual int getFixedLength() = 0;

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
};

#endif // TYPE_H_
