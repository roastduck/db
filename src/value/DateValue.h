#ifndef DATE_VALUE_H_
#define DATE_VALUE_H_

#include "../Value.h"

/** Date value after 1970/1/1
 */
class DateValue : Value
{
    /* TODO: This class needs further implementation
     * NOTE:
     * - We can use time() etc. since date >= 1970/1/1
     * - Be cautious if the user only needs the date part or the time part, or comparation might fail
     */

public:
    ValueID getValueID() override { return ValueID::DATE; }
};

#endif // DATE_VALUE_H_
