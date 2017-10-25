#ifndef PAGE_CACHE_H_
#define PAGE_CACHE_H_

#include <cassert>
#include <array>
#include <string>
#include <utility>
#include <iterator>
#include <type_traits>
#include "PageMgr.h"
#include "PairHash.h"
#include "LinkedHashMap.h"

class PageCache
{
private:
    typedef std::pair<std::string, int> CacheKey;
    typedef std::pair<std::array<unsigned char, PageMgr::PAGE_SIZE>, bool /*dirty?*/> CacheValue;
    LinkedHashMap< CacheKey, CacheValue, PairHash<std::string, int> > map;

    PageMgr &pageMgr;

    static const int DEFAULT_MAX_PAGES = 131072; // Totally 1GB
    const int maxPages;

    void evict()
    {
        auto popped = map.pop();
        if (popped.second.second) // dirty
            pageMgr.write(popped.first.first, popped.first.second, popped.second.first.begin());
    }

    template <bool isConst, typename value_t> // See IterT for template parameters
    value_t *access(const CacheKey &key, int offset)
    {
        assert(offset >= 0 && offset < PageMgr::PAGE_SIZE);
        CacheValue *result = map.find(key);
        if (result)
            return result->first.begin() + offset;

        if (int(map.size()) == maxPages)
            evict();
        CacheValue val;
        pageMgr.read(key.first, key.second, val.first.begin());
        val.second = !isConst;
        CacheValue &ret = map[key] = val;
        return ret.first.begin() + offset;
    }

    template <bool isConst>
    class IterT : public std::iterator<std::random_access_iterator_tag, unsigned char>
    {
    private:
        using value_t = std::conditional<isConst, const unsigned char, unsigned char>;
        PageCache *mgr;
        const CacheKey key;
        int offset;

    public:
        friend IterT<!isConst>;

        IterT() : mgr(NULL) {} // Uninitialized, required to meet STD Standard
        IterT(PageCache *_mgr, const CacheKey &_key, int _offset) : mgr(_mgr), key(_key), offset(_offset) {}

        typename value_t::type &operator*() { return *(mgr->access<isConst, typename value_t::type>(key, offset)); }
        typename value_t::type &operator[](int inc) { return *(mgr->access<isConst, typename value_t::type>(key, offset + inc)); }

        // Functions with template parameters can't be simply defined as friends
        // Or it will cause re-definition
        template <bool c2>
        bool operator==(const IterT<c2> &rhs)
        {
            assert(this->key == rhs.key);
            return this->offset == rhs.offset;
        }
        template <bool c2>
        bool operator!=(const IterT<c2> &rhs)
        {
            assert(this->key == rhs.key);
            return this->offset != rhs.offset;
        }
        template <bool c2>
        bool operator<(const IterT<c2> &rhs)
        {
            assert(this->key == rhs.key);
            return this->offset < rhs.offset;
        }
        template <bool c2>
        bool operator>(const IterT<c2> &rhs)
        {
            assert(this->key == rhs.key);
            return this->offset > rhs.offset;
        }
        template <bool c2>
        bool operator<=(const IterT<c2> &rhs)
        {
            assert(this->key == rhs.key);
            return this->offset <= rhs.offset;
        }
        template <bool c2>
        bool operator>=(const IterT<c2> &rhs)
        {
            assert(this->key == rhs.key);
            return this->offset >= rhs.offset;
        }
        friend IterT operator+(const IterT &iter, int inc) { return IterT(iter.mgr, iter.key, iter.offset + inc); }
        friend IterT operator+(int inc, const IterT &iter) { return IterT(iter.mgr, iter.key, iter.offset + inc); }
        friend IterT operator-(const IterT &iter, int dec) { return IterT(iter.mgr, iter.key, iter.offset - dec); }
        template <bool c2>
        int operator-(const IterT<c2> &rhs)
        {
            assert(this->key == rhs.key);
            return this->offset - rhs.offset;
        }
        IterT &operator+=(int inc) { offset += inc; return *this; }
        IterT &operator-=(int dec) { offset -= dec; return *this; }
        IterT &operator++() { offset++; return *this; }
        IterT &operator--() { offset--; return *this; }
        IterT operator++(int) { IterT ret = *this; offset++; return ret; }
        IterT operator--(int) { IterT ret = *this; offset--; return ret; }
    };
public:
    typedef IterT<false> Iter;
    typedef IterT<true> ConstIter;

    PageCache(PageMgr &_pageMgr, int _maxPages = DEFAULT_MAX_PAGES)
        : pageMgr(_pageMgr), maxPages(_maxPages) {}

    ~PageCache()
    {
        while (!map.empty())
            evict();
    }

    /** Get an iterator to a mutable page
     *  NOTE: This page will be marked dirty no matter if you modify it
     */
    Iter getPage(const std::string &filename, int pageID)
    {
        return Iter(this, CacheKey(filename, pageID), 0);
    }

    /** Get an iterator to a read-only page
     *  This page will not be written back to file
     */
    ConstIter getConstPage(const std::string &filename, int pageID)
    {
        return ConstIter(this, CacheKey(filename, pageID), 0);
    }
};

#endif // PAGE_CACHE_H_
