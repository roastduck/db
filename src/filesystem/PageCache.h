#ifndef PAGE_CACHE_H_
#define PAGE_CACHE_H_

#include <cassert>
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
        CacheKey key;
        unsigned char *buf;
        bool dirty;
    };

    LinkedHashMap< CacheKey, std::shared_ptr<CacheValue>, PairHash<std::string, int> > map;

    PageMgr &pageMgr;

    static const int DEFAULT_MAX_PAGES = 131072; // Totally 1GB
    const int maxPages;

    void evict()
    {
        auto popped = map.pop();
        assert(popped.second->buf != NULL);
        if (popped.second->dirty)
            pageMgr.write(popped.first.first, popped.first.second, popped.second->buf);
        delete[] popped.second->buf;
        popped.second->buf = NULL;
    }

    template <bool isConst>
    std::shared_ptr<CacheValue> create(const CacheKey &key)
    {
        std::shared_ptr<CacheValue> *result = map.find(key);
        if (result)
        {
            (*result)->dirty |= !isConst;
            return *result;
        }
        if (int(map.size()) == maxPages)
            evict();
        std::shared_ptr<CacheValue> ptr(new CacheValue());
        ptr->key = key;
        ptr->buf = new unsigned char[PageMgr::PAGE_SIZE];
        pageMgr.read(ptr->key.first, ptr->key.second, ptr->buf);
        ptr->dirty = !isConst;
        return map[ptr->key] = std::move(ptr);
    }

    template <bool isConst>
    void access(const std::shared_ptr<CacheValue> &ptr)
    {
        assert(!map.find(ptr->key));
        if (int(map.size()) == maxPages)
            evict();
        assert(ptr != nullptr && ptr->buf == NULL);
        ptr->buf = new unsigned char[PageMgr::PAGE_SIZE];
        pageMgr.read(ptr->key.first, ptr->key.second, ptr->buf);
        ptr->dirty |= !isConst;
        map[ptr->key] = ptr;
    }

    template <typename T, bool isConst>
    class IterT : public std::iterator<std::random_access_iterator_tag, T>
    {
    private:
        using value_t = std::conditional<isConst, const T, T>;

        PageCache *mgr;
        int offset;
        std::shared_ptr<CacheValue> val;

    public:
        template <typename, bool> friend class IterT;

        IterT() : mgr(NULL), val(nullptr) {} // Uninitialized, required to meet STD Standard
        IterT(PageCache *_mgr, const CacheKey &_key, int _offset)
            : mgr(_mgr), offset(_offset), val(mgr->create<isConst>(_key))
        {}
        IterT(PageCache *_mgr, int _offset, const std::shared_ptr<CacheValue> &_val)
            : mgr(_mgr), offset(_offset), val(_val)
        {}
        template <typename T2, bool c2>
        IterT(const IterT<T2, c2> &other)
            : mgr(other.mgr), offset(other.offset * sizeof(T2) / sizeof(T)), val(other.val)
        {}

        typename value_t::type &operator*() const
        {
            assert(offset >= 0 && offset * sizeof(T) < PageMgr::PAGE_SIZE);
            assert(val != nullptr);
            if (!val->buf)
                mgr->access<isConst>(val);
            return *((typename value_t::type*)val->buf + offset);
        }

        typename value_t::type &operator[](int inc) const
        {
            assert(offset + inc >= 0 && (offset + inc) * sizeof(T) < PageMgr::PAGE_SIZE);
            assert(val != nullptr);
            if (!val->buf)
                mgr->access<isConst>(val);
            return *((typename value_t::type*)val->buf + offset + inc);
        }

        // Functions with template parameters can't be simply defined as friends
        // Or it will cause re-definition
        template <bool c2> bool operator==(const IterT<T, c2> &rhs) { return this->offset == rhs.offset; }
        template <bool c2> bool operator!=(const IterT<T, c2> &rhs) { return this->offset != rhs.offset; }
        template <bool c2> bool operator<(const IterT<T, c2> &rhs) { return this->offset < rhs.offset; }
        template <bool c2> bool operator>(const IterT<T, c2> &rhs) { return this->offset > rhs.offset; }
        template <bool c2> bool operator<=(const IterT<T, c2> &rhs) { return this->offset <= rhs.offset; }
        template <bool c2> bool operator>=(const IterT<T, c2> &rhs) { return this->offset >= rhs.offset; }
        friend IterT operator+(const IterT &iter, int inc) { return IterT(iter.mgr, iter.offset + inc, iter.val); }
        friend IterT operator+(int inc, const IterT &iter) { return IterT(iter.mgr, iter.offset + inc, iter.val); }
        friend IterT operator-(const IterT &iter, int dec) { return IterT(iter.mgr, iter.offset - dec, iter.val); }
        template <bool c2> int operator-(const IterT<T, c2> &rhs) { return this->offset - rhs.offset; }
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
            {
                unsigned char *&buf = map.staticLookup(k)->buf;
                if (buf)
                {
                    delete[] buf;
                    buf = NULL;
                }
            }
        map.removeIf([&filename](const CacheKey &k){return k.first == filename;});
        pageMgr.destroy(filename);
    }
};

#endif // PAGE_CACHE_H_
