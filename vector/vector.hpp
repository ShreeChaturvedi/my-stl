#pragma once

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <iosfwd>
#include <ostream>

template <typename T> class Vector {
public:
  using iterator = T*;
  using const_iterator = const T*;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // Constructors
  Vector() noexcept;
  explicit Vector(std::size_t initial_capacity);
  Vector(std::initializer_list<T> list);
  Vector(const Vector& other);
  Vector(Vector&& other) noexcept;

  // Assignment
  Vector& operator=(const Vector& other);
  Vector& operator=(Vector&& other) noexcept;

  // Destructor
  ~Vector();

  void swap(Vector& other) noexcept;

  // Accessors
  T& operator[](std::size_t pos) noexcept;
  const T& operator[](std::size_t pos) const noexcept;
  T& at(std::size_t pos);
  const T& at(std::size_t pos) const;

  // Setters
  void push_back(const T& element);
  void push_back(T&& element);

  template <typename... Args> T& emplace_back(Args&&... args);

  iterator insert(const_iterator pos, const T& value);
  iterator insert(const_iterator pos, T&& value);

  template <typename... Args> iterator emplace(const_iterator pos, Args&&... args);

  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);

  // iterators
  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  const_iterator cbegin() const noexcept;
  iterator end() noexcept;
  const_iterator end() const noexcept;
  const_iterator cend() const noexcept;

  reverse_iterator rbegin() noexcept;
  const_reverse_iterator rbegin() const noexcept;
  const_reverse_iterator crbegin() const noexcept;

  reverse_iterator rend() noexcept;
  const_reverse_iterator rend() const noexcept;
  const_reverse_iterator crend() const noexcept;

  void clear() noexcept;
  void reserve(std::size_t capacity);
  void resize(std::size_t size);
  std::size_t capacity() const noexcept;
  T* data() noexcept;
  const T* data() const noexcept;

  T& front();
  const T& front() const;
  T& back();
  const T& back() const;
  void pop_back();

  std::size_t size() const noexcept;
  bool empty() const noexcept;

private:
  void ensure_capacity_for_one_more();
  static T* allocate(std::size_t n);
  void deallocate() noexcept;

  std::size_t size_, capacity_;
  T* data_;
};

template <typename T> std::ostream& operator<<(std::ostream& os, const Vector<T>& vec) {
  os << '[';
  for (std::size_t i = 0; i < vec.size(); ++i) {
    if (i != 0)
      os << ", ";
    os << vec[i];
  }
  return os << ']';
}

#include "vector.tpp"
