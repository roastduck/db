#include <algorithm>
#include "BasePage.h"
#include "filesystem/PageMgr.h"

BasePage::BasePage(PageCache &_pageCache, const std::string &_filename, int _pageID)
    : mutByte(_pageCache.getPage(_filename, _pageID)), constByte(_pageCache.getConstPage(_filename, _pageID)),
      mutShort(mutByte), constShort(constByte), mutInt(mutByte), constInt(constByte)
{}

void BasePage::clear()
{
    std::fill(mutByte, mutByte + PageMgr::PAGE_SIZE, 0);
}

