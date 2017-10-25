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

class MockPageMgr : public PageMgr
{
private:
    std::unordered_map< std::pair<std::string, int>, std::array<unsigned char, PAGE_SIZE>, PairHash<std::string, int> > map;

    void fakeRead(const std::string &filename, int pageID, unsigned char *buf)
    {
        const auto &page = map[std::make_pair(filename, pageID)];
        std::copy(page.begin(), page.end(), buf);
    }

    void fakeWrite(const std::string &filename, int pageID, const unsigned char *buf)
    {
        auto &page = map[std::make_pair(filename, pageID)];
        std::copy(buf, buf + PAGE_SIZE, page.begin());
    }
public:
    MOCK_METHOD3(read, void(const std::string &filename, int pageID, unsigned char *buf));
    MOCK_METHOD3(write, void(const std::string &filename, int pageID, const unsigned char *buf));

    MockPageMgr()
    {
        ON_CALL(*this, read(_, _, _)).WillByDefault(Invoke(this, &MockPageMgr::fakeRead));
        ON_CALL(*this, write(_, _, _)).WillByDefault(Invoke(this, &MockPageMgr::fakeWrite));
    }
};

#endif // MOCK_PAGE_MGR_H_
