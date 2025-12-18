#pragma once

#include <cstddef>
#include <functional>
#include <utility>

#include "utility/unit.hpp"
#include "unordered-multimap/unordered_multimap.hpp"

template <typename K, typename Hash = std::hash<K>, typename KeyEqual = std::equal_to<K>>
class unordered_multiset {
public:
  using key_type = K;
  using value_type = K;
  using size_type = std::size_t;

private:
  using mmap_type = unordered_multimap<K, unit, Hash, KeyEqual>;
  mmap_type map_{};

  template <typename It>
  class base_iterator {
  public:
    using value_type = K;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;
    using pointer = const K*;
    using reference = const K&;

    base_iterator() = default;
    explicit base_iterator(It it) : it_(it) {}

    reference operator*() const { return it_->first; }
    pointer operator->() const { return std::addressof(it_->first); }

    base_iterator& operator++() {
      ++it_;
      return *this;
    }

    base_iterator operator++(int) {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const base_iterator& other) const { return it_ == other.it_; }
    bool operator!=(const base_iterator& other) const { return it_ != other.it_; }

  private:
    It it_{};
  };

public:
  using iterator = base_iterator<typename mmap_type::iterator>;
  using const_iterator = base_iterator<typename mmap_type::const_iterator>;

  unordered_multiset() = default;
  explicit unordered_multiset(size_type bucket_count) : map_(bucket_count) {}

  bool empty() const noexcept { return map_.empty(); }
  size_type size() const noexcept { return map_.size(); }

  iterator begin() { return iterator(map_.begin()); }
  iterator end() { return iterator(map_.end()); }
  const_iterator begin() const { return const_iterator(map_.cbegin()); }
  const_iterator end() const { return const_iterator(map_.cend()); }
  const_iterator cbegin() const { return const_iterator(map_.cbegin()); }
  const_iterator cend() const { return const_iterator(map_.cend()); }

  void clear() noexcept { map_.clear(); }
  void reserve(size_type n) { map_.reserve(n); }

  size_type count(const K& key) const { return map_.count(key); }

  iterator insert(const K& key) {
    map_.insert({key, unit{}});
    return iterator(map_.find(key));
  }

  size_type erase(const K& key) { return map_.erase(key); }
};
