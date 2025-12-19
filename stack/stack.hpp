#pragma once

#include <cstddef>
#include <utility>

#include "vector/vector.hpp"

template <typename T, typename Container = Vector<T>> class Stack {
public:
  bool empty() const noexcept {
    return data_.empty();
  }
  std::size_t size() const noexcept {
    return data_.size();
  }

  T& top() {
    return data_.back();
  }
  const T& top() const {
    return data_.back();
  }

  void push(const T& value) {
    data_.push_back(value);
  }
  void push(T&& value) {
    data_.push_back(std::move(value));
  }

  template <typename... Args> T& emplace(Args&&... args) {
    return data_.emplace_back(std::forward<Args>(args)...);
  }

  void pop() {
    data_.pop_back();
  }

private:
  Container data_{};
};
