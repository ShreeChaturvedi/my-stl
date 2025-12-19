#pragma once

#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "forward-list/forward_list.hpp"
#include "vector/vector.hpp"

template <typename K, typename V, typename Hash = std::hash<K>,
          typename KeyEqual = std::equal_to<K>>
class unordered_map {
public:
  using key_type = K;
  using mapped_type = V;
  using pair_type = std::pair<K, V>;
  using size_type = std::size_t;

  template <typename T_value, typename T_list_iterator> class base_iterator;

  using iterator = base_iterator<pair_type, typename ForwardList<pair_type>::iterator>;
  using const_iterator =
      base_iterator<const pair_type, typename ForwardList<pair_type>::const_iterator>;

  unordered_map() : unordered_map(16) {}
  explicit unordered_map(size_type bucket_count) : buckets_{}, size_(0), max_load_factor_(1.0f) {
    buckets_.resize(bucket_count == 0 ? 1 : bucket_count);
  }

  bool empty() const noexcept {
    return size_ == 0;
  }
  size_type size() const noexcept {
    return size_;
  }
  size_type bucket_count() const noexcept {
    return buckets_.size();
  }

  float load_factor() const noexcept {
    if (bucket_count() == 0)
      return 0.0f;
    return static_cast<float>(size_) / static_cast<float>(bucket_count());
  }

  float max_load_factor() const noexcept {
    return max_load_factor_;
  }
  void max_load_factor(float f) noexcept {
    max_load_factor_ = (f <= 0.0f) ? 1.0f : f;
  }

  void clear() noexcept {
    for (auto& b : buckets_)
      b.clear();
    size_ = 0;
  }

  void reserve(size_type n) {
    const auto needed = static_cast<size_type>(static_cast<float>(n) / max_load_factor_) + 1;
    if (needed > bucket_count())
      rehash(needed);
  }

  void rehash(size_type bucket_count) {
    if (bucket_count < 1)
      bucket_count = 1;
    Vector<ForwardList<pair_type>> next;
    next.resize(bucket_count);

    for (auto& bucket : buckets_) {
      for (auto& kv : bucket) {
        const size_type idx = Hash{}(kv.first) % bucket_count;
        next[idx].push_front(std::move(kv));
      }
    }

    buckets_ = std::move(next);
  }

  void insert(pair_type pair) {
    insert_or_assign(std::move(pair));
  }
  void emplace(K key, V value) {
    insert_or_assign({std::move(key), std::move(value)});
  }

  iterator find(const K& key);
  const_iterator find(const K& key) const;

  void erase(const K& key);

  V& at(const K& key);
  const V& at(const K& key) const;

  V& operator[](const K& key) {
    return try_emplace_default(key);
  }

  iterator begin() {
    return iterator::begin(this);
  }
  iterator end() {
    return iterator::end(this);
  }

  const_iterator begin() const {
    return cbegin();
  }
  const_iterator end() const {
    return cend();
  }

  const_iterator cbegin() const {
    return const_iterator::begin(this);
  }
  const_iterator cend() const {
    return const_iterator::end(this);
  }

private:
  Vector<ForwardList<pair_type>> buckets_;
  size_type size_;
  float max_load_factor_;

  size_type bucket_of(const K& key) const {
    return Hash{}(key) % bucket_count();
  }

  void maybe_rehash_for_insert() {
    const float projected = static_cast<float>(size_ + 1) / static_cast<float>(bucket_count());
    if (projected > max_load_factor_)
      rehash(bucket_count() * 2);
  }

  V& try_emplace_default(const K& key);
  void insert_or_assign(pair_type pair);
};

template <typename K, typename V, typename Hash, typename KeyEqual>
typename unordered_map<K, V, Hash, KeyEqual>::iterator
unordered_map<K, V, Hash, KeyEqual>::find(const K& key) {
  const size_type bucket = bucket_of(key);
  auto& list = buckets_[bucket];
  for (auto it = list.begin(); it != list.end(); ++it) {
    if (KeyEqual{}(it->first, key))
      return iterator(this, bucket, it);
  }
  return end();
}

template <typename K, typename V, typename Hash, typename KeyEqual>
typename unordered_map<K, V, Hash, KeyEqual>::const_iterator
unordered_map<K, V, Hash, KeyEqual>::find(const K& key) const {
  const size_type bucket = bucket_of(key);
  const auto& list = buckets_[bucket];
  for (auto it = list.begin(); it != list.end(); ++it) {
    if (KeyEqual{}(it->first, key))
      return const_iterator(this, bucket, it);
  }
  return cend();
}

template <typename K, typename V, typename Hash, typename KeyEqual>
void unordered_map<K, V, Hash, KeyEqual>::erase(const K& key) {
  const size_type bucket = bucket_of(key);
  auto& list = buckets_[bucket];

  auto before = list.before_begin();
  for (auto it = list.begin(); it != list.end(); ++it) {
    if (KeyEqual{}(it->first, key)) {
      list.erase_after(before);
      --size_;
      return;
    }
    ++before;
  }
}

template <typename K, typename V, typename Hash, typename KeyEqual>
V& unordered_map<K, V, Hash, KeyEqual>::at(const K& key) {
  const size_type bucket = bucket_of(key);
  auto& list = buckets_[bucket];
  for (auto& kv : list) {
    if (KeyEqual{}(kv.first, key))
      return kv.second;
  }
  throw std::out_of_range("unordered_map::at missing key");
}

template <typename K, typename V, typename Hash, typename KeyEqual>
const V& unordered_map<K, V, Hash, KeyEqual>::at(const K& key) const {
  const size_type bucket = bucket_of(key);
  const auto& list = buckets_[bucket];
  for (const auto& kv : list) {
    if (KeyEqual{}(kv.first, key))
      return kv.second;
  }
  throw std::out_of_range("unordered_map::at missing key");
}

template <typename K, typename V, typename Hash, typename KeyEqual>
V& unordered_map<K, V, Hash, KeyEqual>::try_emplace_default(const K& key) {
  const size_type bucket = bucket_of(key);
  auto& list = buckets_[bucket];
  for (auto& kv : list) {
    if (KeyEqual{}(kv.first, key))
      return kv.second;
  }
  maybe_rehash_for_insert();
  const size_type next_bucket = bucket_of(key);
  buckets_[next_bucket].push_front({key, V{}});
  ++size_;
  return buckets_[next_bucket].front().second;
}

template <typename K, typename V, typename Hash, typename KeyEqual>
void unordered_map<K, V, Hash, KeyEqual>::insert_or_assign(pair_type pair) {
  const size_type bucket = bucket_of(pair.first);
  auto& list = buckets_[bucket];
  for (auto& kv : list) {
    if (KeyEqual{}(kv.first, pair.first)) {
      kv.second = std::move(pair.second);
      return;
    }
  }

  maybe_rehash_for_insert();
  const size_type next_bucket = bucket_of(pair.first);
  buckets_[next_bucket].push_front(std::move(pair));
  ++size_;
}

template <typename K, typename V, typename Hash, typename KeyEqual>
template <typename T_value, typename T_list_iterator>
class unordered_map<K, V, Hash, KeyEqual>::base_iterator {
public:
  using value_type = T_value;
  using pointer = T_value*;
  using reference = T_value&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  static base_iterator
  begin(std::conditional_t<std::is_const_v<T_value>, const unordered_map*, unordered_map*> map) {
    base_iterator it(map, 0, {});
    it.skip_empty_buckets();
    return it;
  }

  static base_iterator
  end(std::conditional_t<std::is_const_v<T_value>, const unordered_map*, unordered_map*> map) {
    return base_iterator(map, map->bucket_count(), {});
  }

  base_iterator& operator++() {
    if (bucket_ == map_->bucket_count())
      return *this;

    ++it_;
    if (it_ == bucket_list().end()) {
      ++bucket_;
      skip_empty_buckets();
    }
    return *this;
  }

  base_iterator operator++(int) {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

  reference operator*() const {
    return *it_;
  }
  pointer operator->() const {
    return std::addressof(*it_);
  }

  bool operator==(const base_iterator& other) const {
    return map_ == other.map_ && bucket_ == other.bucket_ &&
           (bucket_ == map_->bucket_count() || it_ == other.it_);
  }

  bool operator!=(const base_iterator& other) const {
    return !(*this == other);
  }

private:
  using map_ptr =
      std::conditional_t<std::is_const_v<T_value>, const unordered_map*, unordered_map*>;

  map_ptr map_;
  size_type bucket_;
  T_list_iterator it_;

public:
  base_iterator(map_ptr map, size_type bucket, T_list_iterator it)
      : map_(map), bucket_(bucket), it_(std::move(it)) {}

private:
  auto& bucket_list() const {
    return map_->buckets_[bucket_];
  }

  void skip_empty_buckets() {
    while (bucket_ < map_->bucket_count() && map_->buckets_[bucket_].empty())
      ++bucket_;
    if (bucket_ >= map_->bucket_count()) {
      bucket_ = map_->bucket_count();
      return;
    }
    it_ = map_->buckets_[bucket_].begin();
  }
};
