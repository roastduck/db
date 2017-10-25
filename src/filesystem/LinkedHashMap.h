#ifndef LINKED_HASH_MAP_H_
#define LINKED_HASH_MAP_H_

#include <cassert>
#include <list>
#include <stdexcept>
#include <unordered_map>

/** Access value by key, or pop the oldest value
 */
template <class K, class V, class Hash = std::hash<K> >
class LinkedHashMap
{
private:
    typedef typename std::list<K>::iterator ListIter;
    struct Node
    {
        V value;
        ListIter listIter;
    };
    typedef typename std::unordered_map<K, Node, Hash>::iterator MapIter;
    std::unordered_map<K, Node, Hash> map;
    std::list<K> list;

    /** Insert if not exists, with value uninitialized
     */
    MapIter findOrCreate(const K &key)
    {
        auto resultPair = map.insert(std::make_pair(key, Node()));
        if (resultPair.second) // inserted
        {
            list.push_front(key);
            resultPair.first->second.listIter = list.begin();
        }
        return resultPair.first;
    }

    /** access a value and update list (pick it to the front)
     */
    V &access(MapIter iter)
    {
        list.erase(iter->second.listIter);
        list.push_front(iter->first);
        iter->second.listIter = list.begin();
        return iter->second.value;
    }

public:
    size_t size() { assert(map.size() == list.size()); return list.size(); }
    bool empty() { return !size(); }

    /** Access or create
     */
    V &operator[](const K &key)
    {
        return access(findOrCreate(key));
    }

    /** Access
     *  @throw std::out_of_range if not exists
     */
    V &at(const K &key)
    {
        auto iter = map.find(key);
        if (iter == map.end())
            throw std::out_of_range("Key not found in LinkedHashMap");
        return access(iter);
    }

    /** Find item
     *  @return : Pointer to value, NULL for not found
     */
    V *find(const K &key)
    {
        auto iter = map.find(key);
        if (iter == map.end())
            return NULL;
        return &access(iter);
    }

    /** Pop the oldest item
     */
    std::pair<K,V> pop()
    {
        assert(size() > 0);
        MapIter iter = map.find(list.back());
        assert(iter != map.end());
        auto ret = std::make_pair(iter->first, iter->second.value);
        map.erase(iter);
        list.pop_back();
        return ret;
    }
};

#endif // LINKED_HASH_MAP_H_
