#pragma once

#include "vector/vector.hpp"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

template <typename Key, typename T, typename Compare = std::less<Key>> class FlatMap {
public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<Key, T>;
  using size_type = std::size_t;
  using iterator = typename Vector<value_type>::iterator;
  using const_iterator = typename Vector<value_type>::const_iterator;

  FlatMap() = default;
  explicit FlatMap(Compare comp) : comp_(std::move(comp)) {}

  iterator begin() noexcept {
    return data_.begin();
  }
  const_iterator begin() const noexcept {
    return data_.begin();
  }
  const_iterator cbegin() const noexcept {
    return data_.cbegin();
  }
  iterator end() noexcept {
    return data_.end();
  }
  const_iterator end() const noexcept {
    return data_.end();
  }
  const_iterator cend() const noexcept {
    return data_.cend();
  }

  size_type size() const noexcept {
    return data_.size();
  }
  bool empty() const noexcept {
    return data_.empty();
  }
  void clear() noexcept {
    data_.clear();
  }
  void reserve(size_type n) {
    data_.reserve(n);
  }

  std::pair<iterator, bool> insert(const value_type& value);
  std::pair<iterator, bool> insert(value_type&& value);

  template <typename... Args> std::pair<iterator, bool> emplace(Args&&... args);

  template <typename... Args> std::pair<iterator, bool> try_emplace(const Key& key, Args&&... args);

  iterator find(const Key& key) noexcept;
  const_iterator find(const Key& key) const noexcept;
  bool contains(const Key& key) const noexcept {
    return find(key) != end();
  }

  T& at(const Key& key);
  const T& at(const Key& key) const;

  T& operator[](const Key& key);
  T& operator[](Key&& key);

  size_type erase(const Key& key);
  iterator erase(const_iterator pos) {
    return data_.erase(pos);
  }

private:
  static bool keys_equal(const Compare& comp, const Key& a, const Key& b) {
    return !comp(a, b) && !comp(b, a);
  }

  iterator lower_bound(const Key& key) noexcept;
  const_iterator lower_bound(const Key& key) const noexcept;

  Vector<value_type> data_;
  Compare comp_{};
};

#include "flat_map.tpp"
