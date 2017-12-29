#ifndef FLOAT_H_
#define FLOAT_H_

#include "../Type.h"

/** FLOAT type
 *  64-bit floating point type
 */
class FloatType : public Type
{
private:
    double val;

public:
    FloatType(int _length = 0) : Type(_length) {}

    TypeID getTypeID() const override { return FLOAT; }

    int getFixedLength() const override { return 8; }

    double getVal() const { return val; }
    void setVal(double _val) { val = _val; }

    std::vector<unsigned char> toBytes() const override;
    void fromBytes(const VectorRef<unsigned char> &bytes) override;

    std::string toString() const override;
    void fromString(const std::string &s) override;

    friend bool operator==(const FloatType&, const FloatType&);
    friend bool operator!=(const FloatType&, const FloatType&);
    friend bool operator<(const FloatType&, const FloatType&);
    friend bool operator<=(const FloatType&, const FloatType&);
    friend bool operator>(const FloatType&, const FloatType&);
    friend bool operator>=(const FloatType&, const FloatType&);
};

#endif // FLOAT_H_
