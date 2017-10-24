#ifndef PAIR_HASH_H_
#define PAIR_HASH_H_

#include <utility>
#include <functional>

template <class T1, class T2>
struct PairHash
{
    std::hash<T1> hashT1;
    std::hash<T2> hashT2;

    size_t operator()(const std::pair<T1, T2> &pair) const
    {
        return (hashT1(pair.first) * 2654435761U) ^ hashT2(pair.second);
    }
};

#endif // PAIR_HASH_H_
