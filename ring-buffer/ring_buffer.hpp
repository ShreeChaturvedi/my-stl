#pragma once

#include <cstddef>
#include <iostream>
#include <array>

template <typename T, std::size_t capacity> class RingBuffer {
public:
  static_assert(capacity > 0, "RingBuffer capacity must be > 0");

  RingBuffer();
  std::size_t push(const T& element);
  T pop();
  T peek() const;
  std::size_t size() const noexcept;
  bool empty() const noexcept;
  bool full() const noexcept;

  // For testing
  friend std::ostream& operator<<(std::ostream& os, const RingBuffer& buffer) {
    if (buffer.empty()) {
      os << "[]";
      return os;
    }

    os << '[';
    for (std::size_t i = buffer.head_; i != buffer.tail_; i = (i + 1) % capacity) {
      os << buffer.buffer_[i] << ", ";
    }
    os << buffer.buffer_[buffer.tail_];
    os << ']';
    return os;
  }

private:
  std::size_t size_;
  std::size_t head_, tail_;
  std::array<T, capacity> buffer_;
};

#include "ring_buffer.tpp"
