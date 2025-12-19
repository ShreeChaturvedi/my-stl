#pragma once

#include "vector/vector.hpp"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

template <typename Key, typename Compare = std::less<Key>> class FlatSet {
public:
  using key_type = Key;
  using value_type = Key;
  using size_type = std::size_t;
  using iterator = typename Vector<value_type>::iterator;
  using const_iterator = typename Vector<value_type>::const_iterator;

  FlatSet() = default;
  explicit FlatSet(Compare comp) : comp_(std::move(comp)) {}

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

  std::pair<iterator, bool> insert(const Key& key);
  std::pair<iterator, bool> insert(Key&& key);

  iterator find(const Key& key) noexcept;
  const_iterator find(const Key& key) const noexcept;
  bool contains(const Key& key) const noexcept {
    return find(key) != end();
  }

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

#include "flat_set.tpp"
