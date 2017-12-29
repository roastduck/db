#ifndef INT_TYPE_H_
#define INT_TYPE_H_

#include "../Type.h"

/** INT type
 *  NOTE: THIS CLASS IS NOT THREAD-SAFE because it uses errno
 */
class IntType : public Type
{
private:
    int val;

public:
    IntType(int _length = 0) : Type(_length) {}

    TypeID getTypeID() const override { return INT; }

    int getFixedLength() const override { return 4; }

    int getVal() const { return val; }
    void setVal(int _val) { val = _val; }

    std::vector<unsigned char> toBytes() const override;
    void fromBytes(const VectorRef<unsigned char> &bytes) override;

    std::string toString() const override;
    void fromString(const std::string &s) override;

    friend bool operator==(const IntType&, const IntType&);
    friend bool operator!=(const IntType&, const IntType&);
    friend bool operator<(const IntType&, const IntType&);
    friend bool operator<=(const IntType&, const IntType&);
    friend bool operator>(const IntType&, const IntType&);
    friend bool operator>=(const IntType&, const IntType&);
};

#endif // INT_TYPE_H_
