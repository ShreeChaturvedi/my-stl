#pragma once

#include <cstddef>
#include <functional>
#include <utility>

#include "heap/heap.hpp"

template <typename T, typename Compare = std::greater<T>, std::size_t branches = 2>
class PriorityQueue {
public:
  bool empty() const noexcept {
    return heap_.empty();
  }
  std::size_t size() const noexcept {
    return heap_.size();
  }

  const T& top() const {
    return heap_.top();
  }

  void push(const T& value) {
    heap_.push(value);
  }

  template <typename... Args> T& emplace(Args&&... args) {
    return heap_.emplace(std::forward<Args>(args)...);
  }

  T pop() {
    return heap_.pop();
  }

  void clear() noexcept {
    heap_.clear();
  }

private:
  Heap<T, Compare, branches> heap_{};
};
