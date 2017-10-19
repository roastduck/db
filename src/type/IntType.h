#ifndef INT_TYPE_H_
#define INT_TYPE_H_

#include "../Type.h"

/** INT type
 */
class IntType : public Type
{
private:
    int val;

public:
    TypeID getTypeID() override { return INT; }

    int getFixedLength() override { return 4; }

    int getVal() const { return val; }
    void setVal(int _val) { val = _val; }

    std::vector<unsigned char> toBytes() const override;
    void fromBytes(const std::vector<unsigned char> &bytes) override;

    std::string toString() const override;
    void fromString(const std::string &s) override;
};

#endif // INT_TYPE_H_
