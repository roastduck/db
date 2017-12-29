#ifndef VECTOR_REF_H_
#define VECTOR_REF_H_

#include <vector>
#include <stdexcept>
#include <initializer_list>
#include "filesystem/PageCache.h"

/** VectorRef provides a copy-free container-like access to a
 *  constant buffer
 */
template <class T>
class VectorRef{};

template <>
class VectorRef<unsigned char>
{
private:
    enum IterType { BARE, CACHED } iterType;
    union IterUnion { const unsigned char *bare; const PageCache::ConstByteIter *cached; } begin;
    int count;

public:
    VectorRef(const unsigned char *_begin, int _count)
        : iterType(BARE), begin{.bare = _begin}, count(_count)
    {}

    VectorRef(const PageCache::ConstByteIter &_begin, int _count)
        : iterType(CACHED), begin{.cached = &_begin}, count(_count)
    {}

    VectorRef(const std::vector<unsigned char> &vec)
        : iterType(BARE), begin{.bare = &vec.front()}, count(vec.size())
    {}

    VectorRef(const std::initializer_list<unsigned char> &init)
        : iterType(BARE), begin{.bare = init.begin()}, count(init.size())
    {}

    const unsigned char &at(int id) const
    {
        if (id < 0 || id >= count)
            throw std::out_of_range("VectorRef::at out of range");
        return iterType == BARE ? begin.bare[id] : (*begin.cached)[id];
    }

    const unsigned char &operator[](int id) const
    {
        return iterType == BARE ? begin.bare[id] : (*begin.cached)[id];
    }

    size_t size() const { return count; }
};

#endif // VECTOR_REF_H_

