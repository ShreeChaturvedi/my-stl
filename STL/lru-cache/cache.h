#pragma once

#include <cstddef>
#include <functional>
#include <list>
#include <optional>
#include <unordered_map>
#include <utility>

template <typename K, typename V>
class LRUCache {
public:
    explicit LRUCache(std::size_t capacity);
    bool insert(const K& key, const V& value);
    std::optional<std::reference_wrapper<const V>> get(const K& key);
    std::size_t size() const noexcept;
private:
    void ensureCapacity();

    std::size_t m_capacity;
    std::list<K> m_queue;
    std::unordered_map<K, std::pair<V, typename std::list<K>::iterator>> m_data;
};

#include "cache.tpp"
