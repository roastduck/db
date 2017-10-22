#ifndef RAW_RECORD_H_
#define RAW_RECORD_H_

#include <vector>

/** Bit representation of a record
 */
struct RawRecord
{
    std::vector<bool> nullColumns; /// Which columns are null
    std::vector<unsigned char> fixedLengths; /// Bytes of fixed-length part
    std::vector< std::vector<unsigned char> > varLengths; /// Bytes of variable-length part
};

#endif // RAW_RECORD_H_
