#pragma once

#include <cstddef>
#include <functional>
#include <optional>
#include <utility>

#include "list/list.hpp"
#include "unordered-map/unordered_map.hpp"

template <typename K, typename V>
class LRUCache {
public:
    explicit LRUCache(std::size_t capacity);
    bool insert(const K& key, const V& value);
    std::optional<std::reference_wrapper<const V>> get(const K& key);
    std::size_t size() const noexcept;
private:
    void ensure_capacity();

    struct Entry {
        V value;
        typename List<K>::iterator it;
    };

    std::size_t capacity_;
    List<K> queue_;
    unordered_map<K, Entry> data_;
};

#include "lru_cache.tpp"
