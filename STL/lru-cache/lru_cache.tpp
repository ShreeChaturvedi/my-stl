// Intentionally header-only (included from lru_cache.hpp).

#include "lru_cache.hpp"

template <typename K, typename V>
LRUCache<K, V>::LRUCache(std::size_t capacity) : 
    capacity_(capacity), queue_{}, data_{} {}

template <typename K, typename V>
bool LRUCache<K, V>::insert(const K& key, const V& value) {
    auto it = data_.find(key);
    if (it != data_.end()) {
        auto& context = it->second;
        context.first = value;
        queue_.splice(queue_.begin(), queue_, context.second);
        return false;
    }
    ensure_capacity();
    queue_.push_front(key);
    data_.emplace(key, std::make_pair(value, queue_.begin()));
    return true;
}

template <typename K, typename V>
std::optional<std::reference_wrapper<const V>> LRUCache<K, V>::get(const K& key) {
    auto it = data_.find(key);
    if (it == data_.end()) return std::nullopt;
    auto& context = it->second;
    queue_.erase(context.second);
    queue_.push_front(key);
    context.second = queue_.begin();
    return context.first;
}

template <typename K, typename V>
std::size_t LRUCache<K, V>::size() const noexcept {
    return data_.size();
}

template <typename K, typename V>
void LRUCache<K, V>::ensure_capacity() {
    if (data_.size() < capacity_) return;
    data_.erase(queue_.back());
    queue_.pop_back();
}
