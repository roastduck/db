#ifndef DATE_TYPE_H_
#define DATE_TYPE_H_

#include "../Type.h"
#include "../exception/InvalidLiteralException.h"

/** Date from 1970/1/1 to the running day
 *  We use a 16-bit integer to save it
 *  Plase refer to issue #2 for date format
 *  NOTE: THIS CLASS IS NOT THREAD-SAFE because it uses C time functions,
 *  but changing to function with _r can solve it
 */
class DateType : public Type
{
public:
    typedef time_t date_t;

private:
    date_t val; /// NOTE: `val` might not be multiple of 86400, because of time zones

public:
    TypeID getTypeID() override { return DATE; }

    int getFixedLength() override { return 8; }

    date_t getVal() const { return val; }
    void setVal(date_t _val) { val = _val; }

    static date_t today();
    static void parseDate(date_t date, int &year, int &month, int &day);

    std::vector<unsigned char> toBytes() const override;
    void fromBytes(const std::vector<unsigned char> &bytes) override;

    std::string toString() const override;
    void fromString(const std::string &s) override;
};

#endif // DATE_TYPE_H_
