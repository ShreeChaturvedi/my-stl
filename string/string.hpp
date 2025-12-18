#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <utility>

template <typename CharT>
class basic_string {
public:
  using value_type = CharT;
  using size_type = std::size_t;
  using pointer = CharT*;
  using const_pointer = const CharT*;
  using reference = CharT&;
  using const_reference = const CharT&;
  using iterator = CharT*;
  using const_iterator = const CharT*;

  basic_string() noexcept;
  basic_string(const CharT* s);
  basic_string(std::basic_string_view<CharT> sv);

  basic_string(const basic_string& other);
  basic_string(basic_string&& other) noexcept;
  ~basic_string();

  basic_string& operator=(const basic_string& other);
  basic_string& operator=(basic_string&& other) noexcept;

  void clear() noexcept;
  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type capacity() const noexcept;

  const_pointer c_str() const noexcept;
  const_pointer data() const noexcept;
  pointer data() noexcept;

  iterator begin() noexcept;
  iterator end() noexcept;
  const_iterator begin() const noexcept;
  const_iterator end() const noexcept;
  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;

  reference operator[](size_type i) noexcept;
  const_reference operator[](size_type i) const noexcept;
  reference at(size_type i);
  const_reference at(size_type i) const;

  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;

  void reserve(size_type new_capacity);
  void push_back(CharT ch);

  basic_string& append(std::basic_string_view<CharT> sv);

  basic_string& operator+=(std::basic_string_view<CharT> sv);
  basic_string& operator+=(const basic_string& other);
  basic_string& operator+=(CharT ch);

  std::basic_string_view<CharT> view() const noexcept;

  friend bool operator==(const basic_string& a, const basic_string& b) noexcept { return a.view() == b.view(); }

private:
  static constexpr size_type sso_capacity_ = (23 / sizeof(CharT)) > 0 ? (23 / sizeof(CharT)) : 1;

  std::allocator<CharT> alloc_{};
  pointer data_;
  size_type size_;
  size_type capacity_;
  CharT sso_[sso_capacity_ + 1];

  bool is_sso() const noexcept { return data_ == sso_; }
  void set_sso_empty() noexcept;
  void ensure_capacity_for_one_more();
  void reallocate(size_type new_capacity);
};

using string = basic_string<char>;

#include "string.tpp"
