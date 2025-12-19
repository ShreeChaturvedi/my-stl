#pragma once

#include <cstddef>
#include <functional>
#include <utility>

#include "rb-tree/rb_tree.hpp"

template <typename K, typename Compare = std::less<K>> class set {
public:
  using key_type = K;
  using value_type = K;
  using size_type = std::size_t;

private:
  struct key_of_value {
    const K& operator()(const K& v) const noexcept {
      return v;
    }
  };

  using tree_type = RbTree<value_type, key_of_value, Compare, false>;
  tree_type tree_{};

public:
  using iterator = typename tree_type::iterator;
  using const_iterator = typename tree_type::const_iterator;

  set() = default;
  explicit set(Compare comp) : tree_(std::move(comp)) {}

  bool empty() const noexcept {
    return tree_.empty();
  }
  size_type size() const noexcept {
    return tree_.size();
  }

  iterator begin() noexcept {
    return tree_.begin();
  }
  const_iterator begin() const noexcept {
    return tree_.begin();
  }
  const_iterator cbegin() const noexcept {
    return tree_.cbegin();
  }

  iterator end() noexcept {
    return tree_.end();
  }
  const_iterator end() const noexcept {
    return tree_.end();
  }
  const_iterator cend() const noexcept {
    return tree_.cend();
  }

  void clear() noexcept {
    tree_.clear();
  }

  iterator find(const K& key) noexcept {
    return tree_.find(key);
  }
  const_iterator find(const K& key) const noexcept {
    return tree_.find(key);
  }
  bool contains(const K& key) const noexcept {
    return find(key) != end();
  }

  iterator lower_bound(const K& key) noexcept {
    return tree_.lower_bound(key);
  }
  const_iterator lower_bound(const K& key) const noexcept {
    return tree_.lower_bound(key);
  }

  iterator upper_bound(const K& key) noexcept {
    return tree_.upper_bound(key);
  }
  const_iterator upper_bound(const K& key) const noexcept {
    return tree_.upper_bound(key);
  }

  std::pair<iterator, iterator> equal_range(const K& key) noexcept {
    return {lower_bound(key), upper_bound(key)};
  }

  std::pair<const_iterator, const_iterator> equal_range(const K& key) const noexcept {
    return {lower_bound(key), upper_bound(key)};
  }

  std::pair<iterator, bool> insert(value_type value) {
    return tree_.insert_unique(std::move(value));
  }

  void erase(const K& key) {
    auto it = find(key);
    if (it != end())
      tree_.erase(it);
  }

  iterator erase(iterator pos) {
    return tree_.erase(pos);
  }
};

template <typename K, typename Compare = std::less<K>> class multiset {
public:
  using key_type = K;
  using value_type = K;
  using size_type = std::size_t;

private:
  struct key_of_value {
    const K& operator()(const K& v) const noexcept {
      return v;
    }
  };

  using tree_type = RbTree<value_type, key_of_value, Compare, true>;
  tree_type tree_{};

public:
  using iterator = typename tree_type::iterator;
  using const_iterator = typename tree_type::const_iterator;

  multiset() = default;
  explicit multiset(Compare comp) : tree_(std::move(comp)) {}

  bool empty() const noexcept {
    return tree_.empty();
  }
  size_type size() const noexcept {
    return tree_.size();
  }

  iterator begin() noexcept {
    return tree_.begin();
  }
  const_iterator begin() const noexcept {
    return tree_.begin();
  }
  const_iterator cbegin() const noexcept {
    return tree_.cbegin();
  }

  iterator end() noexcept {
    return tree_.end();
  }
  const_iterator end() const noexcept {
    return tree_.end();
  }
  const_iterator cend() const noexcept {
    return tree_.cend();
  }

  void clear() noexcept {
    tree_.clear();
  }

  iterator find(const K& key) noexcept {
    return tree_.find(key);
  }
  const_iterator find(const K& key) const noexcept {
    return tree_.find(key);
  }

  iterator lower_bound(const K& key) noexcept {
    return tree_.lower_bound(key);
  }
  const_iterator lower_bound(const K& key) const noexcept {
    return tree_.lower_bound(key);
  }

  iterator upper_bound(const K& key) noexcept {
    return tree_.upper_bound(key);
  }
  const_iterator upper_bound(const K& key) const noexcept {
    return tree_.upper_bound(key);
  }

  std::pair<iterator, iterator> equal_range(const K& key) noexcept {
    return {lower_bound(key), upper_bound(key)};
  }

  std::pair<const_iterator, const_iterator> equal_range(const K& key) const noexcept {
    return {lower_bound(key), upper_bound(key)};
  }

  iterator insert(value_type value) {
    return tree_.insert_multi(std::move(value));
  }

  void erase_one(const K& key) {
    auto it = find(key);
    if (it != end())
      tree_.erase(it);
  }

  size_type erase_all(const K& key) {
    size_type erased = 0;
    auto [first, last] = equal_range(key);
    while (first != last) {
      first = tree_.erase(first);
      ++erased;
    }
    return erased;
  }

  iterator erase(iterator pos) {
    return tree_.erase(pos);
  }
};
