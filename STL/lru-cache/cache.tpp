template <typename K, typename V>
LRUCache<K, V>::LRUCache(std::size_t capacity) : 
    m_capacity(capacity), m_queue{}, m_data{} {}

template <typename K, typename V>
bool LRUCache<K, V>::insert(const K& key, const V& value) {
    auto it = m_data.find(key);
    if (it != m_data.end()) {
        auto& context = it->second;
        context.first = value;
        m_queue.splice(m_queue.begin(), m_queue, context.second);
        return false;
    }
    ensureCapacity();
    m_queue.push_front(key);
    m_data.emplace(key, std::make_pair(value, m_queue.begin()));
    return true;
}

template <typename K, typename V>
std::optional<std::reference_wrapper<const V>> LRUCache<K, V>::get(const K& key) {
    auto it = m_data.find(key);
    if (it == m_data.end()) return std::nullopt;
    auto& context = it->second;
    m_queue.erase(context.second);
    m_queue.push_front(key);
    context.second = m_queue.begin();
    return context.first;
}

template <typename K, typename V>
std::size_t LRUCache<K, V>::size() const noexcept {
    return m_data.size();
}

template <typename K, typename V>
void LRUCache<K, V>::ensureCapacity() {
    if (m_data.size() < m_capacity) return;
    m_data.erase(m_queue.back());
    m_queue.pop_back();
    return;
}
