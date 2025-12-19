#pragma once

#include <memory>
#include <type_traits>
#include <utility>

template <typename T> class unique_ptr {
public:
  using pointer = T*;
  using element_type = T;
  using deleter_type = std::default_delete<T>;

  unique_ptr() noexcept : ptr_(nullptr) {}
  explicit unique_ptr(pointer ptr) noexcept : ptr_(ptr) {}
  unique_ptr(const unique_ptr& other) = delete;
  unique_ptr(unique_ptr&& other) noexcept : ptr_(std::exchange(other.ptr_, nullptr)) {}
  unique_ptr& operator=(const unique_ptr& other) = delete;
  unique_ptr& operator=(unique_ptr&& other) noexcept {
    if (this == &other)
      return *this;
    reset(std::exchange(other.ptr_, nullptr));
    return *this;
  }

  void swap(unique_ptr& other) noexcept {
    std::swap(ptr_, other.ptr_);
  }

  T& operator*() {
    return *ptr_;
  }

  const T& operator*() const {
    return *ptr_;
  }

  pointer operator->() noexcept {
    return ptr_;
  }
  pointer operator->() const noexcept {
    return ptr_;
  }

  pointer get() const noexcept {
    return ptr_;
  }

  explicit operator bool() const noexcept {
    return ptr_ != nullptr;
  }

  pointer release() noexcept {
    return std::exchange(ptr_, nullptr);
  }

  void reset(pointer new_ptr = nullptr) noexcept {
    if (ptr_ == new_ptr)
      return;
    if (ptr_)
      deleter_type{}(ptr_);
    ptr_ = new_ptr;
  }

  ~unique_ptr() {
    reset();
  }

private:
  pointer ptr_;
};
