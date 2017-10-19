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
    TypeID getTypeID() override { return TypeID::FLOAT; }

    int getFixedLength() override { return 8; }

    double getVal() const { return val; }
    void setVal(double _val) { val = _ val; }

    std::vector<unsigned char> toBytes() const override;
    void fromBytes(const std::vector<unsigned char> &bytes) override;

    std::string toString() const override;
    void fromString(const std::string &s) override;
};

#endif // FLOAT_H_
