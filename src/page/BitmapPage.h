#ifndef BITMAP_PAGE_H_
#define BITMAP_PAGE_H_

#include <cassert>
#include <cstring>
#include "../BasePage.h"
#include "../filesystem/PageMgr.h"

/** The whole page is used as a bitmap
 */
class BitmapPage : public BasePage
{
private:
    int oneCount;

public:
    BitmapPage(PageCache &_cache, const std::string &_filename, int _pageID)
        : BasePage(_cache, _filename, _pageID)
    {
        oneCount = 0;
        for (int i = 0; i * 4 < PageMgr::PAGE_SIZE; i++)
            oneCount += __builtin_popcount(constInt[i]);
    }

    /** Get number of ones
     */
    int count()
    {
        return oneCount;
    }

    /** Set i-th bit to 1
     */
    void set(int i)
    {
        unsigned int &ref = mutInt[i >> 5]; // i / 32
        ref = ((ref & ~(1 << (i & 31))) | (1 << (i & 31)));
    }

    /** Set i-th bit to 0
     */
    void reset(int i)
    {
        unsigned int &ref = mutInt[i >> 5];
        ref = (ref & ~(1 << (i & 31)));
    }

    /** Get i-th bit
     */
    bool get(int i)
    {
        return (constInt[i >> 5] >> (i & 31)) & 1;
    }

    /** Rank of first 0
     *  @return -1 if all bits are 1
     */
    int first0()
    {
        if (count() == PageMgr::PAGE_SIZE * 8)
            return -1;
        for (int i = 0; i * 4 < PageMgr::PAGE_SIZE; i++)
        {
            unsigned int x = constInt[i];
            if (x == 0xffffffff)
                continue;
            return ffs(~x) - 1 + 32 * i; // ffs indexes from 1
        }
        assert(false);
    }
};

#endif // BITMAP_PAGE_H_
