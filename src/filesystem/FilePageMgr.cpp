#include <cerrno>
#include <cassert>
#include "FilePageMgr.h"
#include "../exception/FileError.h"

FilePageMgr::FilePageMgr(const std::string &_baseDir)
    : baseDir(_baseDir + "/")
{}

FilePageMgr::~FilePageMgr()
{
    for (auto item : openedFile)
        fclose(item.second);
}

FILE *FilePageMgr::openWithOffset(const std::string &filename, int offset)
{
    FILE *f;
    auto iter = openedFile.find(filename);
    if (iter != openedFile.end())
        f = iter->second;
    else
    {
        errno = 0;
        f = fopen((baseDir + filename).c_str(), "r+b");
        if (!f && errno == ENOENT)
        {
            f = fopen((baseDir + filename).c_str(), "w");
            fclose(f);
            f = fopen((baseDir + filename).c_str(), "r+b");
        }
        if (!f)
            throw FileError(baseDir + filename);
        openedFile[filename] = f;
    }
    fseek(f, offset, SEEK_SET);
    return f;
}

void FilePageMgr::read(const std::string &filename, int pageID, unsigned char *buf)
{
    assert(pageID >= 0);
    FILE *f = openWithOffset(filename, pageID * PAGE_SIZE);
    fread(buf, 1, PAGE_SIZE, f);
}

void FilePageMgr::write(const std::string &filename, int pageID, const unsigned char *buf)
{
    assert(pageID >= 0);
    FILE *f = openWithOffset(filename, pageID * PAGE_SIZE);
    fwrite(buf, 1, PAGE_SIZE, f);
}

void FilePageMgr::destroy(const std::string &filename)
{
    if (remove((baseDir + filename).c_str()))
        throw FileError(baseDir + filename);
}

