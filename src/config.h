#ifndef CONFIG_H_
#define CONFIG_H_

#include "page/ListPage.h"
#include "filesystem/PageMgr.h"

const int MAX_IDENTIFIER_LEN = 64;
const int MAX_COLUMN_NUM = 16;
const int MAX_INDEX_NUM = 16;
const int MAX_CHECK_LEN = 64;

// Because B-tree requires at least 2 entries at one node so it's easy to handle
const int MAX_RECORD_BYTES = (PageMgr::PAGE_SIZE - ListPage::HEADER_SIZE) / 2;

#endif // CONFIG_H_
