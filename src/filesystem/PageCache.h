#ifndef PAGE_CACHE_H_
#define PAGE_CACHE_H_

#include <cassert>
#include <array>
#include <memory>
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

    struct CacheValue
    {
        std::array<unsigned char, PageMgr::PAGE_SIZE> buf;
        bool dirty;
        std::shared_ptr<bool> deleted; // This indicates you must to re-read from file
    };

    LinkedHashMap< CacheKey, CacheValue, PairHash<std::string, int> > map;

    PageMgr &pageMgr;

    static const int DEFAULT_MAX_PAGES = 131072; // Totally 1GB
    const int maxPages;

    void evict()
    {
        auto popped = map.pop();
        *(popped.second.deleted) = true;
        if (popped.second.dirty)
            pageMgr.write(popped.first.first, popped.first.second, popped.second.buf.begin());
    }

    template <bool isConst>
    CacheValue &access(const CacheKey &key)
    {
        CacheValue *result = map.find(key);
        // Still need to check this. Althogh the original position is deleted, a new one might have been in place
        if (result)
        {
            result->dirty |= !isConst; // Mark it dirty once a mutable iterator connects the cache
            return *result;
        }

        if (int(map.size()) == maxPages)
            evict();
        CacheValue val;
        pageMgr.read(key.first, key.second, val.buf.begin());
        val.dirty = !isConst;
        val.deleted = std::shared_ptr<bool>(new bool(false));
        return map[key] = std::move(val);
    }

    template <typename T, bool isConst>
    class IterT : public std::iterator<std::random_access_iterator_tag, T>
    {
    private:
        using value_t = std::conditional<isConst, const T, T>;

        PageCache *mgr;
        CacheKey key;
        int offset;
        typename value_t::type *buf;
        std::shared_ptr<bool> deleted;

    public:
        template <typename, bool> friend class IterT;

        IterT() : mgr(NULL), buf(NULL), deleted(nullptr) {} // Uninitialized, required to meet STD Standard
        IterT(PageCache *_mgr, const CacheKey &_key, int _offset) : mgr(_mgr), key(_key), offset(_offset) {}

        template <typename T2, bool c2>
        IterT(const IterT<T2, c2> &other)
            : mgr(other.mgr), key(other.key), offset(other.offset * sizeof(T2) / sizeof(T)),
              buf((typename value_t::type*)other.buf), deleted(other.deleted)
            {}

        typename value_t::type &operator*()
        {
            assert(offset >= 0 && offset * sizeof(T) < PageMgr::PAGE_SIZE);
            if (deleted == nullptr || *deleted)
            {
                CacheValue &ret = mgr->access<isConst>(key);
                deleted = ret.deleted;
                buf = (typename value_t::type*)ret.buf.begin();
            }
            return *(buf + offset);
        }

        typename value_t::type &operator[](int inc) { return *(*this + inc); }

        // Functions with template parameters can't be simply defined as friends
        // Or it will cause re-definition
        template <bool c2>
        bool operator==(const IterT<T, c2> &rhs)
        {
            assert(this->key == rhs.key);
            return this->offset == rhs.offset;
        }
        template <bool c2>
        bool operator!=(const IterT<T, c2> &rhs)
        {
            assert(this->key == rhs.key);
            return this->offset != rhs.offset;
        }
        template <bool c2>
        bool operator<(const IterT<T, c2> &rhs)
        {
            assert(this->key == rhs.key);
            return this->offset < rhs.offset;
        }
        template <bool c2>
        bool operator>(const IterT<T, c2> &rhs)
        {
            assert(this->key == rhs.key);
            return this->offset > rhs.offset;
        }
        template <bool c2>
        bool operator<=(const IterT<T, c2> &rhs)
        {
            assert(this->key == rhs.key);
            return this->offset <= rhs.offset;
        }
        template <bool c2>
        bool operator>=(const IterT<T, c2> &rhs)
        {
            assert(this->key == rhs.key);
            return this->offset >= rhs.offset;
        }
        friend IterT operator+(const IterT &iter, int inc) { return IterT(iter.mgr, iter.key, iter.offset + inc); }
        friend IterT operator+(int inc, const IterT &iter) { return IterT(iter.mgr, iter.key, iter.offset + inc); }
        friend IterT operator-(const IterT &iter, int dec) { return IterT(iter.mgr, iter.key, iter.offset - dec); }
        template <bool c2>
        int operator-(const IterT<T, c2> &rhs)
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
    typedef IterT<unsigned char, false> MutByteIter;
    typedef IterT<unsigned char, true> ConstByteIter;
    typedef IterT<unsigned short, false> MutShortIter;
    typedef IterT<unsigned short, true> ConstShortIter;
    typedef IterT<unsigned int, false> MutIntIter;
    typedef IterT<unsigned int, true> ConstIntIter;

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
    MutByteIter getPage(const std::string &filename, int pageID)
    {
        return MutByteIter(this, CacheKey(filename, pageID), 0);
    }

    /** Get an iterator to a read-only page
     *  This page will not be written back to file
     */
    ConstByteIter getConstPage(const std::string &filename, int pageID)
    {
        return ConstByteIter(this, CacheKey(filename, pageID), 0);
    }

    /** Drop all cache about a file and delete that file
     */
    void destroy(const std::string &filename)
    {
        for (const CacheKey &k : map.getKeys())
            if (k.first == filename)
                *(map.staticLookup(k).deleted) = true;
        map.removeIf([&filename](const CacheKey &k){return k.first == filename;});
        pageMgr.destroy(filename);
    }
};

#endif // PAGE_CACHE_H_
