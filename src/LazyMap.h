#ifndef LAZY_MAP_H_
#define LAZY_MAP_H_

#include <functional>

template <class K, class V>
class LazyMap
{
public:
    typedef std::function<V(const K&)> Callable;

private:
    Callable callback;

public:
    LazyMap(const Callable &_callback) : callback(_callback) {}

    V at(const K &key) const { return callback(key); }
};

#endif // LAZY_MAP_H_
