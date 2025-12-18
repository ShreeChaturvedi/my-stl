template <typename K, typename V>
LRUCache<K, V>::LRUCache(std::size_t capacity) : 
    capacity_(capacity), queue_{}, data_{} {}

template <typename K, typename V>
bool LRUCache<K, V>::insert(const K& key, const V& value) {
    if (capacity_ == 0) return false;
    auto it = data_.find(key);
    if (it != data_.end()) {
        auto& entry = it->second;
        entry.value = value;
        queue_.move_to_front(entry.it);
        return false;
    }
    ensure_capacity();
    auto node_it = queue_.push_front(key);
    data_.insert({key, Entry{value, node_it}});
    return true;
}

template <typename K, typename V>
std::optional<std::reference_wrapper<const V>> LRUCache<K, V>::get(const K& key) {
    if (capacity_ == 0) return std::nullopt;
    auto it = data_.find(key);
    if (it == data_.end()) return std::nullopt;
    auto& entry = it->second;
    queue_.move_to_front(entry.it);
    return entry.value;
}

template <typename K, typename V>
std::size_t LRUCache<K, V>::size() const noexcept {
    return data_.size();
}

template <typename K, typename V>
void LRUCache<K, V>::ensure_capacity() {
    if (capacity_ == 0) return;
    if (data_.size() < capacity_) return;
    const K evict_key = queue_.back();
    data_.erase(evict_key);
    queue_.pop_back();
}
