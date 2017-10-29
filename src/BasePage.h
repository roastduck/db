#ifndef BASE_PAGE_H_
#define BASE_PAGE_H_

#include "filesystem/PageCache.h"

/** Base class for all pages
 */
class BasePage
{
protected:
    PageCache::MutByteIter mutByte;
    PageCache::ConstByteIter constByte;
    PageCache::MutShortIter mutShort;
    PageCache::ConstShortIter constShort;
    PageCache::MutIntIter mutInt;
    PageCache::ConstIntIter constInt;

public:
    BasePage(PageCache &_pageCache, const std::string &_filename, int _pageID);
};

#endif // BASE_PAGE_H_
