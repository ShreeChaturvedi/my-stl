#include <type_traits>

template <typename Key, typename T, typename Compare>
typename FlatMap<Key, T, Compare>::iterator
FlatMap<Key, T, Compare>::lower_bound(const Key& key) noexcept {
  return std::lower_bound(data_.begin(), data_.end(), key,
                          [this](const value_type& v, const Key& k) { return comp_(v.first, k); });
}

template <typename Key, typename T, typename Compare>
typename FlatMap<Key, T, Compare>::const_iterator
FlatMap<Key, T, Compare>::lower_bound(const Key& key) const noexcept {
  return std::lower_bound(data_.begin(), data_.end(), key,
                          [this](const value_type& v, const Key& k) { return comp_(v.first, k); });
}

template <typename Key, typename T, typename Compare>
std::pair<typename FlatMap<Key, T, Compare>::iterator, bool>
FlatMap<Key, T, Compare>::insert(const value_type& value) {
  if (data_.empty()) {
    data_.push_back(value);
    return {data_.end() - 1, true};
  }

  const auto& last_key = data_.back().first;
  if (keys_equal(comp_, last_key, value.first))
    return {data_.end() - 1, false};
  if (comp_(last_key, value.first)) {
    data_.push_back(value);
    return {data_.end() - 1, true};
  }

  auto it = lower_bound(value.first);
  if (it != end() && keys_equal(comp_, it->first, value.first))
    return {it, false};
  it = data_.insert(it, value);
  return {it, true};
}

template <typename Key, typename T, typename Compare>
std::pair<typename FlatMap<Key, T, Compare>::iterator, bool>
FlatMap<Key, T, Compare>::insert(value_type&& value) {
  const Key& key = value.first;
  if (data_.empty()) {
    data_.push_back(std::move(value));
    return {data_.end() - 1, true};
  }

  const auto& last_key = data_.back().first;
  if (keys_equal(comp_, last_key, key))
    return {data_.end() - 1, false};
  if (comp_(last_key, key)) {
    data_.push_back(std::move(value));
    return {data_.end() - 1, true};
  }

  auto it = lower_bound(key);
  if (it != end() && keys_equal(comp_, it->first, key))
    return {it, false};
  it = data_.insert(it, std::move(value));
  return {it, true};
}

template <typename Key, typename T, typename Compare>
template <typename... Args>
std::pair<typename FlatMap<Key, T, Compare>::iterator, bool>
FlatMap<Key, T, Compare>::emplace(Args&&... args) {
  value_type value(std::forward<Args>(args)...);
  return insert(std::move(value));
}

template <typename Key, typename T, typename Compare>
template <typename... Args>
std::pair<typename FlatMap<Key, T, Compare>::iterator, bool>
FlatMap<Key, T, Compare>::try_emplace(const Key& key, Args&&... args) {
  if (data_.empty()) {
    data_.emplace_back(key, T(std::forward<Args>(args)...));
    return {data_.end() - 1, true};
  }

  const auto& last_key = data_.back().first;
  if (keys_equal(comp_, last_key, key))
    return {data_.end() - 1, false};
  if (comp_(last_key, key)) {
    data_.emplace_back(key, T(std::forward<Args>(args)...));
    return {data_.end() - 1, true};
  }

  auto it = lower_bound(key);
  if (it != end() && keys_equal(comp_, it->first, key))
    return {it, false};
  it = data_.emplace(it, key, T(std::forward<Args>(args)...));
  return {it, true};
}

template <typename Key, typename T, typename Compare>
typename FlatMap<Key, T, Compare>::iterator
FlatMap<Key, T, Compare>::find(const Key& key) noexcept {
  auto it = lower_bound(key);
  if (it != end() && keys_equal(comp_, it->first, key))
    return it;
  return end();
}

template <typename Key, typename T, typename Compare>
typename FlatMap<Key, T, Compare>::const_iterator
FlatMap<Key, T, Compare>::find(const Key& key) const noexcept {
  auto it = lower_bound(key);
  if (it != end() && keys_equal(comp_, it->first, key))
    return it;
  return end();
}

template <typename Key, typename T, typename Compare>
T& FlatMap<Key, T, Compare>::at(const Key& key) {
  auto it = find(key);
  if (it == end())
    throw std::out_of_range("FlatMap::at key not found");
  return it->second;
}

template <typename Key, typename T, typename Compare>
const T& FlatMap<Key, T, Compare>::at(const Key& key) const {
  auto it = find(key);
  if (it == end())
    throw std::out_of_range("FlatMap::at key not found");
  return it->second;
}

template <typename Key, typename T, typename Compare>
T& FlatMap<Key, T, Compare>::operator[](const Key& key) {
  static_assert(std::is_default_constructible_v<T>,
                "FlatMap::operator[] requires default constructible T");
  auto [it, inserted] = try_emplace(key);
  (void)inserted;
  return it->second;
}

template <typename Key, typename T, typename Compare>
T& FlatMap<Key, T, Compare>::operator[](Key&& key) {
  static_assert(std::is_default_constructible_v<T>,
                "FlatMap::operator[] requires default constructible T");
  if (data_.empty()) {
    data_.emplace_back(std::move(key), T{});
    return data_.back().second;
  }

  const auto& last_key = data_.back().first;
  if (keys_equal(comp_, last_key, key))
    return data_.back().second;
  if (comp_(last_key, key)) {
    data_.emplace_back(std::move(key), T{});
    return data_.back().second;
  }

  auto it = lower_bound(key);
  if (it != end() && keys_equal(comp_, it->first, key))
    return it->second;
  it = data_.emplace(it, std::move(key), T{});
  return it->second;
}

template <typename Key, typename T, typename Compare>
typename FlatMap<Key, T, Compare>::size_type FlatMap<Key, T, Compare>::erase(const Key& key) {
  auto it = find(key);
  if (it == end())
    return 0;
  data_.erase(it);
  return 1;
}
