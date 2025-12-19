#include <cstddef>
#include <array>
#include <stdexcept>

template <typename T, std::size_t capacity>
RingBuffer<T, capacity>::RingBuffer() : size_(0), head_(0), tail_(capacity - 1), buffer_{} {}

template <typename T, std::size_t capacity>
std::size_t RingBuffer<T, capacity>::push(const T& element) {
  if (full())
    throw std::out_of_range("Cannot push to a full buffer");
  tail_ = (tail_ + 1) % capacity;
  buffer_[tail_] = element;
  return ++size_;
}

template <typename T, std::size_t capacity> T RingBuffer<T, capacity>::pop() {
  T element = peek();
  head_ = (head_ + 1) % capacity;
  --size_;
  return element;
}

template <typename T, std::size_t capacity> T RingBuffer<T, capacity>::peek() const {
  if (empty())
    throw std::out_of_range("Cannot pop from an empty buffer");
  return buffer_[head_];
}

template <typename T, std::size_t capacity>
std::size_t RingBuffer<T, capacity>::size() const noexcept {
  return size_;
};

template <typename T, std::size_t capacity> bool RingBuffer<T, capacity>::empty() const noexcept {
  return size_ == 0;
};

template <typename T, std::size_t capacity> bool RingBuffer<T, capacity>::full() const noexcept {
  return size_ == capacity;
};
