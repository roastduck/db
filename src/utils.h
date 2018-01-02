#ifndef UTILS_H_
#define UTILS_H_

#include <cassert>
#include <string>
#include <vector>
#include <memory>
#include "Type.h"
#include "type/IntType.h"
#include "type/CharType.h"
#include "type/FloatType.h"

std::string commaJoin(const std::vector<std::string> &strs);
std::vector<std::string> commaSep(const std::string &str);

std::string toLower(std::string s);

/** Find the first index i in [begin, end) satisfying !tooLow(i)
 */
template <class Callback>
int binSearch(int begin, int end, const Callback &tooLow)
{
    int l = begin, r = end;
    while (l < r)
    {
        int mid = (l + r) / 2;
        if (tooLow(mid))
            l = mid + 1;
        else
            r = mid;
    }
    assert(l - 1 < begin || tooLow(l - 1));
    assert(l >= end || !tooLow(l));
    return l;
}

inline IntType &asInt(const std::unique_ptr<Type> &p)
{
    return *dynamic_cast<IntType*>(p.get());
}

inline FloatType &asFloat(const std::unique_ptr<Type> &p)
{
    return *dynamic_cast<FloatType*>(p.get());
}

inline CharType &asChar(const std::unique_ptr<Type> &p)
{
    return *dynamic_cast<CharType*>(p.get());
}

#endif // UTILS_H_
