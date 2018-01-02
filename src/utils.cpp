#include <algorithm>
#include "utils.h"

std::string commaJoin(const std::vector<std::string> &strs)
{
    std::string ret;
    for (const std::string &s : strs)
    {
        if (!ret.empty()) ret.push_back(',');
        ret.append(s);
    }
    return ret;
}

std::vector<std::string> commaSep(const std::string &str)
{
    std::vector<std::string> ret = {""};
    for (char c : str)
        if (c == ',')
            ret.push_back("");
        else
            ret.back().push_back(c);
    return ret;
}

std::string toLower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), tolower);
    return s;
}

