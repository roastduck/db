#include <string>

#ifndef PAGE_MRG_H_
#define PAGE_MRG_H_

/** Interface for page I/O
 */
class PageMgr
{
public:
    static const int PAGE_SIZE = 8 * 1024; // bytes

    virtual ~PageMgr() {}

    /** Read from page and write to buf
     */
    virtual void read(const std::string &filename, int pageID, unsigned char *buf) = 0;

    /** Read from buf and write to page
     */
    virtual void write(const std::string &filename, int pageID, const unsigned char *buf) = 0;
};

#endif // PAGE_MRG_H_
