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
class unordered_multimap {
public:
  using key_type = K;
  using mapped_type = V;
  using value_type = std::pair<const K, V>;
  using size_type = std::size_t;

  template <typename T_value, typename T_list_iterator> class base_iterator;

  using iterator = base_iterator<value_type, typename ForwardList<value_type>::iterator>;
  using const_iterator =
      base_iterator<const value_type, typename ForwardList<value_type>::const_iterator>;

  unordered_multimap() : unordered_multimap(16) {}

  explicit unordered_multimap(size_type bucket_count)
      : buckets_{}, size_(0), max_load_factor_(1.0f) {
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

  void rehash(size_type new_bucket_count) {
    if (new_bucket_count < 1)
      new_bucket_count = 1;
    Vector<ForwardList<value_type>> next;
    next.resize(new_bucket_count);
    size_ = 0;

    for (auto& bucket : buckets_) {
      for (auto& kv : bucket) {
        insert_into(next, std::move(kv));
      }
      bucket.clear();
    }

    buckets_ = std::move(next);
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

  iterator find(const K& key) {
    const size_type b = bucket_of(key);
    return iterator::from_bucket(this, b, find_in_bucket(b, key));
  }
  const_iterator find(const K& key) const {
    const size_type b = bucket_of(key);
    return const_iterator::from_bucket(this, b, find_in_bucket(b, key));
  }

  size_type count(const K& key) const {
    const size_type b = bucket_of(key);
    const auto& list = buckets_[b];
    size_type n = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
      if (KeyEqual{}(it->first, key))
        ++n;
      else if (n != 0)
        break;
    }
    return n;
  }

  std::pair<iterator, iterator> equal_range(const K& key) {
    const size_type b = bucket_of(key);
    auto it = find_in_bucket(b, key);
    if (it == buckets_[b].end())
      return {end(), end()};

    auto last = it;
    while (last != buckets_[b].end() && KeyEqual{}(last->first, key))
      ++last;
    return {iterator::from_bucket(this, b, it), iterator::from_bucket(this, b, last)};
  }

  std::pair<const_iterator, const_iterator> equal_range(const K& key) const {
    const size_type b = bucket_of(key);
    auto it = find_in_bucket(b, key);
    if (it == buckets_[b].end())
      return {cend(), cend()};

    auto last = it;
    while (last != buckets_[b].end() && KeyEqual{}(last->first, key))
      ++last;
    return {const_iterator::from_bucket(this, b, it), const_iterator::from_bucket(this, b, last)};
  }

  iterator insert(value_type value) {
    maybe_rehash_for_insert();
    insert_into(buckets_, std::move(value));
    return find(value.first);
  }

  template <typename... Args> iterator emplace(Args&&... args) {
    return insert(value_type(std::forward<Args>(args)...));
  }

  size_type erase(const K& key) {
    const size_type b = bucket_of(key);
    auto& list = buckets_[b];

    size_type erased = 0;
    auto before = list.before_begin();
    for (auto it = list.begin(); it != list.end();) {
      if (!KeyEqual{}(it->first, key)) {
        if (erased != 0)
          break;
        ++before;
        ++it;
        continue;
      }
      it = list.erase_after(before);
      ++erased;
      --size_;
    }
    return erased;
  }

private:
  Vector<ForwardList<value_type>> buckets_;
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

  template <typename Buckets> void insert_into(Buckets& buckets, value_type value) {
    const size_type b = Hash{}(value.first) % buckets.size();
    auto& list = buckets[b];

    auto before = list.before_begin();
    auto it = list.begin();
    auto last_equal = list.before_begin();
    bool found = false;

    for (; it != list.end(); ++it) {
      if (KeyEqual{}(it->first, value.first)) {
        found = true;
        last_equal = before;
      } else if (found) {
        break;
      }
      ++before;
    }

    if (!found) {
      list.push_front(std::move(value));
    } else {
      list.insert_after(last_equal, std::move(value));
    }
    ++size_;
  }

  typename ForwardList<value_type>::iterator find_in_bucket(size_type b, const K& key) {
    auto& list = buckets_[b];
    for (auto it = list.begin(); it != list.end(); ++it) {
      if (KeyEqual{}(it->first, key))
        return it;
    }
    return list.end();
  }

  typename ForwardList<value_type>::const_iterator find_in_bucket(size_type b, const K& key) const {
    const auto& list = buckets_[b];
    for (auto it = list.begin(); it != list.end(); ++it) {
      if (KeyEqual{}(it->first, key))
        return it;
    }
    return list.end();
  }
};

template <typename K, typename V, typename Hash, typename KeyEqual>
template <typename T_value, typename T_list_iterator>
class unordered_multimap<K, V, Hash, KeyEqual>::base_iterator {
public:
  using value_type = T_value;
  using pointer = T_value*;
  using reference = T_value&;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  using map_ptr =
      std::conditional_t<std::is_const_v<T_value>, const unordered_multimap*, unordered_multimap*>;

  static base_iterator begin(map_ptr map) {
    base_iterator it(map, 0, {});
    it.skip_empty_buckets();
    return it;
  }

  static base_iterator end(map_ptr map) {
    return base_iterator(map, map->bucket_count(), {});
  }

  static base_iterator from_bucket(map_ptr map, std::size_t bucket, T_list_iterator it) {
    base_iterator out(map, bucket, it);
    if (bucket >= map->bucket_count())
      return end(map);
    if (it == map->buckets_[bucket].end()) {
      ++out.bucket_;
      out.skip_empty_buckets();
    }
    return out;
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
  map_ptr map_;
  std::size_t bucket_;
  T_list_iterator it_;

  base_iterator(map_ptr map, std::size_t bucket, T_list_iterator it)
      : map_(map), bucket_(bucket), it_(std::move(it)) {}

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
