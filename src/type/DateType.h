#ifndef DATE_TYPE_H_
#define DATE_TYPE_H_

#include "../Type.h"

/** Date from 1970/1/1 to the running day
 *  We use a 16-bit integer to save it
 */
class DateType : public Type
{
private:
    short val;

public:
    TypeID getTypeID() override { return TypeID::DATE; }

    int getFixedLength() override { return 2; }

    short getVal() const { return val; }
    void setVal(short _val) { val = _val; }

    std::vector<unsigned char> toBytes() const override;
    void fromBytes(const std::vector<unsigned char> &bytes) override;

    std::string toString() const override;
    void fromString(const std::string &s) override;
};

#endif // DATE_TYPE_H_
