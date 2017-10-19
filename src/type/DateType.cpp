#include <ctime>
#include <cstring>
#include <cassert>
#include <iomanip>
#include <sstream>
#include "DateType.h"
#include "../exception/InvalidLiteralException.h"

DateType::date_t DateType::today()
{
    return time(0);
}

void DateType::parseDate(date_t date, int &year, int &month, int &day)
{
    struct tm *timeInfo = localtime(&date); // don't delete timeInfo
    year = timeInfo->tm_year + 1900;
    month = timeInfo->tm_mon + 1;
    day = timeInfo->tm_mday;
}

std::vector<unsigned char> DateType::toBytes() const
{
    std::vector<unsigned char> ret(8);
    for (int i = 0; i < 8; i++)
        ret[i] = *((unsigned char*)(&val) + i);
    return ret;
}

void DateType::fromBytes(const std::vector<unsigned char> &bytes)
{
    assert(bytes.size() == 8);
    for (int i = 0; i < 8; i++)
        *((unsigned char*)(&val) + i) = bytes[i];
}

std::string DateType::toString() const
{
    std::ostringstream os;
    int year, month, day;
    parseDate(val, year, month, day);
    os << std::setw(4) << std::setfill('0') << year << '-'
       << std::setw(2) << std::setfill('0') << month << '-'
       << std::setw(2) << std::setfill('0') << day;
    return os.str();
}

void DateType::fromString(const std::string &s)
{
    struct tm timeInfo;
    memset(&timeInfo, 0, sizeof(struct tm));
    char *en = strptime(s.c_str(), "%Y-%m-%d", &timeInfo);
    if (!en || *en != '\0')
        throw InvalidLiteralException(s, DATE);
    if (timeInfo.tm_year + 1900 < 1970)
        throw InvalidLiteralException(s, DATE);
    val = mktime(&timeInfo);
    if (val > today())
        throw InvalidLiteralException(s, DATE);
}

