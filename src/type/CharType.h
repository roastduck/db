#ifndef CHAR_TYPE_H_
#define CHAR_TYPE_H_

#include <cassert>
#include <string>
#include "../Type.h"

/** CHAR type
 *  A CHAR is a fixed-length string
 */
class CharType : public Type
{
private:
    std::string val;

public:
    CharType(int _length) : Type(_length) { assert(_length > 0); }

    TypeID getTypeID() const override { return CHAR; }

    int getFixedLength() const override { return length; }

    const std::string &getVal() const { return val; }
    void setVal(const std::string &_val) { assert(int(_val.length()) <= length); val = _val; }

    std::vector<unsigned char> toBytes() const override;
    void fromBytes(const std::vector<unsigned char> &bytes) override;

    std::string toString() const override { return val; }
    void fromString(const std::string &s) override;

    friend bool operator==(const CharType&, const CharType&);
    friend bool operator!=(const CharType&, const CharType&);
    friend bool operator<(const CharType&, const CharType&);
    friend bool operator<=(const CharType&, const CharType&);
    friend bool operator>(const CharType&, const CharType&);
    friend bool operator>=(const CharType&, const CharType&);
};

#endif // CHAR_TYPE_H_
