#ifndef FILE_PAGE_MGR_H_
#define FILE_PAGE_MGR_H_

#include <cstdio>
#include <unordered_map>
#include "PageMgr.h"

/** Actual page manager that handles file I/O
 */
class FilePageMgr : public PageMgr
{
private:
    const std::string baseDir;
    std::unordered_map<std::string, FILE*> openedFile;

    FILE *openWithOffset(const std::string &filename, int offset);

public:
    FilePageMgr(const std::string &_baseDir);
    ~FilePageMgr();

    void read(const std::string &filename, int pageID, unsigned char *buf) override;
    void write(const std::string &filename, int pageID, const unsigned char *buf) override;
};

#endif // FILE_PAGE_MGR_H_
