#ifndef MOCK_PAGE_MGR_H_
#define MOCK_PAGE_MGR_H_

#include <array>
#include <utility>
#include <algorithm>
#include <unordered_map>
#include "filesystem/PageMgr.h"
#include "filesystem/PairHash.h"

#include "gmock/gmock.h"

using namespace testing;

class RamPageMgr : public PageMgr
{
private:
    std::unordered_map< std::pair<std::string, int>, std::array<unsigned char, PAGE_SIZE>, PairHash<std::string, int> > map;

public:
    void read(const std::string &filename, int pageID, unsigned char *buf) override
    {
        const auto &page = map[std::make_pair(filename, pageID)];
        std::copy(page.begin(), page.end(), buf);
    }

    void write(const std::string &filename, int pageID, const unsigned char *buf) override
    {
        auto &page = map[std::make_pair(filename, pageID)];
        std::copy(buf, buf + PAGE_SIZE, page.begin());
    }
};

class MockPageMgr : public RamPageMgr
{
public:
    MOCK_METHOD3(read, void(const std::string &filename, int pageID, unsigned char *buf));
    MOCK_METHOD3(write, void(const std::string &filename, int pageID, const unsigned char *buf));
};

#endif // MOCK_PAGE_MGR_H_
