#ifndef MOCK_PAGE_MGR_H_
#define MOCK_PAGE_MGR_H_

#include <array>
#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_map>
#include "filesystem/PageMgr.h"
#include "filesystem/PairHash.h"

#include "gmock/gmock.h"

using namespace testing;

class MockPageMgr : public PageMgr
{
private:
    std::unordered_map< std::pair<std::string, int>, std::array<unsigned char, PAGE_SIZE>, PairHash<std::string, int> > map;

    void fakeRead(const std::string &filename, int pageID, unsigned char *buf)
    {
        auto iter = map.find(std::make_pair(filename, pageID));
        if (iter == map.end())
        {
            std::array<unsigned char, PAGE_SIZE> empty;
            std::fill(empty.begin(), empty.end(), 0);
            iter = map.insert(std::make_pair(std::make_pair(filename, pageID), empty)).first;
        }
        const auto &page = iter->second;
        std::copy(page.begin(), page.end(), buf);
    }

    void fakeWrite(const std::string &filename, int pageID, const unsigned char *buf)
    {
        auto &page = map[std::make_pair(filename, pageID)];
        std::copy(buf, buf + PAGE_SIZE, page.begin());
    }

    void fakeDestroy(const std::string &filename)
    {
        std::vector<int> pages;
        for (const auto &item : map)
            if (item.first.first == filename)
                pages.push_back(item.first.second);
        for (int page : pages)
            map.erase(std::make_pair(filename, page));
    }
public:
    MOCK_METHOD3(read, void(const std::string &filename, int pageID, unsigned char *buf));
    MOCK_METHOD3(write, void(const std::string &filename, int pageID, const unsigned char *buf));
    MOCK_METHOD1(destroy, void(const std::string &filename));

    MockPageMgr()
    {
        ON_CALL(*this, read(_, _, _)).WillByDefault(Invoke(this, &MockPageMgr::fakeRead));
        ON_CALL(*this, write(_, _, _)).WillByDefault(Invoke(this, &MockPageMgr::fakeWrite));
        ON_CALL(*this, destroy(_)).WillByDefault(Invoke(this, &MockPageMgr::fakeDestroy));
    }
};

#endif // MOCK_PAGE_MGR_H_
