#ifndef VARCHAR_TYPE_H_
#define VARCHAR_TYPE_H_

#include <string>
#include "../Type.h"

class VarcharType : public Type
{
private:
    std::string val;

public:
    VarcharType(int length);

    TypeID getTypeID() override { return TypeID::VARCHAR; }

    int getFixedLength() override { return 0; }

    const std::string &getVal() const { return val; }
    void setVal(const std::string &_val) { val = _val; }

    std::vector<unsigned char> toBytes() const override;
    void fromBytes(const std::vector<unsigned char> &bytes) override;

    std::string toString() const override { return val; }
    void fromString(const std::string &s) override { val = s; }
    // TODO: Are there any length limit of VARCHAR?
};

#endif // VARCHAR_TYPE_H_
