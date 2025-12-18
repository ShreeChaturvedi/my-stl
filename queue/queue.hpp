#pragma once

#include <cstddef>
#include <utility>

#include "list/list.hpp"

template <typename T, typename Container = List<T>>
class Queue {
public:
  bool empty() const noexcept { return data_.empty(); }
  std::size_t size() const noexcept { return data_.size(); }

  T& front() { return data_.front(); }
  const T& front() const { return data_.front(); }

  T& back() { return data_.back(); }
  const T& back() const { return data_.back(); }

  void push(const T& value) { data_.push_back(value); }
  void push(T&& value) { data_.push_back(std::move(value)); }

  template <typename... Args>
  void emplace(Args&&... args) {
    data_.push_back(T(std::forward<Args>(args)...));
  }

  void pop() { data_.pop_front(); }

private:
  Container data_{};
};

