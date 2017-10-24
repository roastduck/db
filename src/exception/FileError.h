#ifndef FILE_ERROR_H_
#define FILE_ERROR_H_

#include <stdexcept>

class FileError : public std::runtime_error
{
public:
    FileError(const std::string &filename)
        : std::runtime_error("Cannot open file \"" + filename + "\"")
        {}
};

#endif // FILE_ERROR_H_
