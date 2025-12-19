#pragma once

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

template <typename T, std::size_t InlineCapacity = 8> class SmallVector {
public:
  using value_type = T;
  using size_type = std::size_t;
  using iterator = T*;
  using const_iterator = const T*;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  SmallVector() noexcept;
  SmallVector(std::initializer_list<T> list);
  SmallVector(const SmallVector& other);
  SmallVector(SmallVector&& other) noexcept(std::is_nothrow_move_constructible_v<T>);

  SmallVector& operator=(const SmallVector& other);
  SmallVector& operator=(SmallVector&& other) noexcept(std::is_nothrow_move_constructible_v<T>);

  ~SmallVector();

  iterator begin() noexcept {
    return data_;
  }
  const_iterator begin() const noexcept {
    return data_;
  }
  const_iterator cbegin() const noexcept {
    return data_;
  }
  iterator end() noexcept {
    return data_ + size_;
  }
  const_iterator end() const noexcept {
    return data_ + size_;
  }
  const_iterator cend() const noexcept {
    return data_ + size_;
  }

  reverse_iterator rbegin() noexcept {
    return reverse_iterator(end());
  }
  const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  reverse_iterator rend() noexcept {
    return reverse_iterator(begin());
  }
  const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }
  const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(begin());
  }

  size_type size() const noexcept {
    return size_;
  }
  bool empty() const noexcept {
    return size_ == 0;
  }
  size_type capacity() const noexcept {
    return capacity_;
  }

  T* data() noexcept {
    return data_;
  }
  const T* data() const noexcept {
    return data_;
  }

  bool using_inline_storage() const noexcept {
    return data_ == inline_data();
  }

  T& operator[](size_type i) noexcept {
    return data_[i];
  }
  const T& operator[](size_type i) const noexcept {
    return data_[i];
  }
  T& at(size_type i);
  const T& at(size_type i) const;

  T& front();
  const T& front() const;
  T& back();
  const T& back() const;

  void clear() noexcept;
  void reserve(size_type new_capacity);

  void push_back(const T& value) {
    emplace_back(value);
  }
  void push_back(T&& value) {
    emplace_back(std::move(value));
  }

  template <typename... Args> T& emplace_back(Args&&... args);

  void pop_back();

  iterator insert(const_iterator pos, const T& value) {
    return emplace(pos, value);
  }
  iterator insert(const_iterator pos, T&& value) {
    return emplace(pos, std::move(value));
  }

  template <typename... Args> iterator emplace(const_iterator pos, Args&&... args);

  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);

  void swap(SmallVector& other) noexcept(std::is_nothrow_move_constructible_v<T>);

private:
  static constexpr size_type inline_capacity() noexcept {
    return InlineCapacity;
  }

  T* inline_data() noexcept;
  const T* inline_data() const noexcept;

  static T* allocate(size_type n);
  static void deallocate(T* p, size_type n) noexcept;

  void grow_to(size_type new_capacity);

  size_type size_ = 0;
  size_type capacity_ = 0;
  T* data_ = nullptr;

  alignas(T) std::byte inline_storage_[InlineCapacity == 0 ? 1 : sizeof(T) * InlineCapacity];
};

#include "small_vector.tpp"
