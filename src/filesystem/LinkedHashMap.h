#ifndef LINKED_HASH_MAP_H_
#define LINKED_HASH_MAP_H_

#include <cassert>
#include <list>
#include <stdexcept>
#include <unordered_map>

/** Access value by key, or pop the oldest value
 */
template <class K, class V>
class LinkedHashMap
{
private:
    typedef typename std::list<K>::iterator ListIter;
    struct Node
    {
        V value;
        ListIter listIter;
    };
    typedef typename std::unordered_map<K, Node>::iterator MapIter;
    std::unordered_map<K, Node> map;
    std::list<K> list;

    /** Insert if not exists, with value uninitialized
     */
    MapIter findOrCreate(const K &key)
    {
        auto resultPair = map.insert(std::make_pair(key, Node()));
        if (resultPair.second) // inserted
        {
            list.push_frond(key);
            resultPair.first->second.listIter = list.begin();
        }
        return resultPair.first;
    }

    /** Find item
     *  @throws std::out_of_range
     */
    MapIter find(const K &key)
    {
        MapIter iter = map.find(key);
        if (iter == map.end())
            throw std::out_of_range("Key not found in LinkedHashMap");
        return iter;
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
        return acccess(findOrCreate(key));
    }

    /** Access, throw std::out_of_range if not exists
     */
    V &at(const K &key)
    {
        return access(find(key));
    }

    /** Pop the oldest item
     */
    void pop()
    {
        assert(size() > 0);
        map.erase(list.back());
        list.pop_back();
    }
};

#endif // LINKED_HASH_MAP_H_
