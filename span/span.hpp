#pragma once

#include <cstddef>
#include <stdexcept>
#include <type_traits>

#include "vector/vector.hpp"

template <typename T>
class Span {
public:
  using element_type = T;
  using value_type = std::remove_cv_t<T>;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using pointer = T*;
  using reference = T&;
  using iterator = T*;
  using const_iterator = const T*;

  constexpr Span() noexcept : data_(nullptr), size_(0) {}
  constexpr Span(pointer data, size_type size) noexcept : data_(data), size_(size) {}

  template <size_type N>
  constexpr Span(T (&arr)[N]) noexcept : data_(arr), size_(N) {}

  template <typename U>
  requires(std::is_convertible_v<U (*)[], T (*)[]>)
  constexpr Span(Vector<U>& v) noexcept : data_(v.data()), size_(v.size()) {}

  template <typename U>
  requires(std::is_convertible_v<const U (*)[], T (*)[]>)
  constexpr Span(const Vector<U>& v) noexcept : data_(v.data()), size_(v.size()) {}

  constexpr pointer data() const noexcept { return data_; }
  constexpr size_type size() const noexcept { return size_; }
  constexpr bool empty() const noexcept { return size_ == 0; }

  constexpr iterator begin() const noexcept { return data_; }
  constexpr iterator end() const noexcept { return data_ + size_; }

  constexpr reference operator[](size_type i) const noexcept { return data_[i]; }

  reference at(size_type i) const {
    if (i >= size_) throw std::out_of_range("Span::at out of range");
    return data_[i];
  }

  reference front() const {
    if (empty()) throw std::out_of_range("Span::front on empty");
    return data_[0];
  }

  reference back() const {
    if (empty()) throw std::out_of_range("Span::back on empty");
    return data_[size_ - 1];
  }

  Span subspan(size_type offset, size_type count = static_cast<size_type>(-1)) const {
    if (offset > size_) throw std::out_of_range("Span::subspan offset out of range");
    const size_type remaining = size_ - offset;
    const size_type n = (count == static_cast<size_type>(-1) || count > remaining) ? remaining : count;
    return Span(data_ + offset, n);
  }

private:
  pointer data_;
  size_type size_;
};

