#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

template <typename T> class Deque {
public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  class iterator;
  class const_iterator;

  Deque() noexcept;
  Deque(const Deque& other);
  Deque(Deque&& other) noexcept;
  ~Deque();

  Deque& operator=(const Deque& other);
  Deque& operator=(Deque&& other) noexcept;

  bool empty() const noexcept;
  size_type size() const noexcept;

  T& operator[](size_type i) noexcept;
  const T& operator[](size_type i) const noexcept;

  T& at(size_type i);
  const T& at(size_type i) const;

  T& front();
  const T& front() const;
  T& back();
  const T& back() const;

  void clear() noexcept;

  void push_back(const T& value);
  void push_back(T&& value);
  void push_front(const T& value);
  void push_front(T&& value);

  void pop_back();
  void pop_front();

  iterator begin() noexcept;
  iterator end() noexcept;
  const_iterator begin() const noexcept;
  const_iterator end() const noexcept;
  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;

private:
  std::allocator<T> alloc_{};
  T* data_;
  size_type size_;
  size_type capacity_;
  size_type head_;

  size_type phys_index(size_type i) const noexcept;
  void ensure_capacity_for_one_more();
  void grow(size_type new_capacity);
  void destroy_all() noexcept;
  void deallocate() noexcept;
};

template <typename T> class Deque<T>::iterator {
public:
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = T*;
  using reference = T&;
  using iterator_category = std::random_access_iterator_tag;

  iterator() noexcept : deque_(nullptr), index_(0) {}

  reference operator*() const {
    return (*deque_)[index_];
  }
  pointer operator->() const {
    return std::addressof(**this);
  }

  iterator& operator++() {
    ++index_;
    return *this;
  }
  iterator operator++(int) {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }
  iterator& operator--() {
    --index_;
    return *this;
  }
  iterator operator--(int) {
    auto tmp = *this;
    --(*this);
    return tmp;
  }

  iterator& operator+=(difference_type n) {
    index_ += static_cast<size_type>(n);
    return *this;
  }
  iterator& operator-=(difference_type n) {
    index_ -= static_cast<size_type>(n);
    return *this;
  }

  friend iterator operator+(iterator it, difference_type n) {
    return it += n;
  }
  friend iterator operator-(iterator it, difference_type n) {
    return it -= n;
  }
  friend difference_type operator-(const iterator& a, const iterator& b) {
    return static_cast<difference_type>(a.index_) - static_cast<difference_type>(b.index_);
  }

  reference operator[](difference_type n) const {
    return (*deque_)[index_ + static_cast<size_type>(n)];
  }

  bool operator==(const iterator& other) const {
    return deque_ == other.deque_ && index_ == other.index_;
  }
  bool operator!=(const iterator& other) const {
    return !(*this == other);
  }
  bool operator<(const iterator& other) const {
    return index_ < other.index_;
  }
  bool operator>(const iterator& other) const {
    return index_ > other.index_;
  }
  bool operator<=(const iterator& other) const {
    return index_ <= other.index_;
  }
  bool operator>=(const iterator& other) const {
    return index_ >= other.index_;
  }

private:
  friend class Deque;
  friend class const_iterator;
  iterator(Deque* d, size_type i) noexcept : deque_(d), index_(i) {}

  Deque* deque_;
  size_type index_;
};

template <typename T> class Deque<T>::const_iterator {
public:
  using value_type = const T;
  using difference_type = std::ptrdiff_t;
  using pointer = const T*;
  using reference = const T&;
  using iterator_category = std::random_access_iterator_tag;

  const_iterator() noexcept : deque_(nullptr), index_(0) {}
  const_iterator(iterator it) noexcept : deque_(it.deque_), index_(it.index_) {}

  reference operator*() const {
    return (*deque_)[index_];
  }
  pointer operator->() const {
    return std::addressof(**this);
  }

  const_iterator& operator++() {
    ++index_;
    return *this;
  }
  const_iterator operator++(int) {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }
  const_iterator& operator--() {
    --index_;
    return *this;
  }
  const_iterator operator--(int) {
    auto tmp = *this;
    --(*this);
    return tmp;
  }

  const_iterator& operator+=(difference_type n) {
    index_ += static_cast<size_type>(n);
    return *this;
  }
  const_iterator& operator-=(difference_type n) {
    index_ -= static_cast<size_type>(n);
    return *this;
  }

  friend const_iterator operator+(const_iterator it, difference_type n) {
    return it += n;
  }
  friend const_iterator operator-(const_iterator it, difference_type n) {
    return it -= n;
  }
  friend difference_type operator-(const const_iterator& a, const const_iterator& b) {
    return static_cast<difference_type>(a.index_) - static_cast<difference_type>(b.index_);
  }

  reference operator[](difference_type n) const {
    return (*deque_)[index_ + static_cast<size_type>(n)];
  }

  bool operator==(const const_iterator& other) const {
    return deque_ == other.deque_ && index_ == other.index_;
  }
  bool operator!=(const const_iterator& other) const {
    return !(*this == other);
  }
  bool operator<(const const_iterator& other) const {
    return index_ < other.index_;
  }
  bool operator>(const const_iterator& other) const {
    return index_ > other.index_;
  }
  bool operator<=(const const_iterator& other) const {
    return index_ <= other.index_;
  }
  bool operator>=(const const_iterator& other) const {
    return index_ >= other.index_;
  }

private:
  friend class Deque;
  const_iterator(const Deque* d, size_type i) noexcept : deque_(d), index_(i) {}

  const Deque* deque_;
  size_type index_;
};

#include "deque.tpp"
