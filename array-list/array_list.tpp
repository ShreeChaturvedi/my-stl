#include "array_list.hpp"

#include <stdexcept>
#include <algorithm>

inline constexpr std::size_t DEFAULT_CAPACITY = 16;

template <typename T> ArrayList<T>::ArrayList() : ArrayList(DEFAULT_CAPACITY) {}

template <typename T>
ArrayList<T>::ArrayList(std::size_t initial_capacity)
    : size_(0), capacity_(initial_capacity), data_(new T[capacity_]) {}

template <typename T>
ArrayList<T>::ArrayList(std::size_t length, T value)
    : size_(length), capacity_(length), data_(new T[capacity_]) {
  for (std::size_t pos = 0; pos < length; ++pos)
    data_[pos] = value;
}

template <typename T>
ArrayList<T>::ArrayList(const ArrayList& other) : size_(other.size_), capacity_(other.capacity_) {
  data_ = new T[capacity_];
  for (std::size_t pos = 0; pos < size_; ++pos)
    data_[pos] = other.data_[pos];
}

template <typename T>
ArrayList<T>::ArrayList(ArrayList&& other) : size_(other.size_), capacity_(other.capacity_) {
  data_ = other.data_;

  other.size_ = 0;
  other.capacity_ = 0;
  other.data_ = nullptr;
}

template <typename T> ArrayList<T>::~ArrayList() {
  delete[] data_;
}

template <typename T> ArrayList<T>& ArrayList<T>::operator=(const ArrayList& other) {
  if (this == &other)
    return *this;
  size_ = other.size_;

  if (size_ > capacity_) {
    T* new_array = new T[size_];
    std::swap(data_, new_array);
    // almost forgot but we don't do memory leaks around here :)
    delete[] new_array;
    capacity_ = size_;
  }

  for (std::size_t pos = 0; pos < size_; ++pos)
    data_[pos] = other.data_[pos];

  return *this;
}

template <typename T> ArrayList<T>& ArrayList<T>::operator=(ArrayList<T>&& other) noexcept {
  if (this == &other)
    return *this;

  delete[] data_;

  data_ = other.data_;
  size_ = other.size_;
  capacity_ = other.capacity_;

  other.size_ = 0;
  other.capacity_ = 0;
  other.data_ = nullptr;

  return *this;
}

template <typename T> void ArrayList<T>::add(const T& element) {
  ensure_capacity();
  data_[size_++] = element;
}

template <typename T> T ArrayList<T>::remove(std::size_t index) {
  if (index >= size_)
    throw std::out_of_range("Index out of range");
  T element = data_[index];
  --size_;
  for (std::size_t pos = index; pos < size_; ++pos)
    data_[pos] = data_[pos + 1];
  return element;
}

template <typename T> void ArrayList<T>::insert(std::size_t index, T element) {
  if (index > size_)
    throw std::out_of_range("Index out of range");
  ensure_capacity();
  for (std::size_t pos = size_; pos > index; --pos)
    data_[pos] = data_[pos - 1];
  data_[index] = element;
  ++size_;
}

template <typename T> T& ArrayList<T>::operator[](std::size_t index) {
  if (index >= size_)
    throw std::out_of_range("Index out of range");
  return data_[index];
}

template <typename T> const T& ArrayList<T>::operator[](std::size_t index) const {
  if (index >= size_)
    throw std::out_of_range("Index out of range");
  return data_[index];
}

template <typename T> void ArrayList<T>::reserve(std::size_t capacity) {
  if (capacity_ >= capacity)
    return;
  capacity_ = capacity;
  T* new_array = copy_array();
  std::swap(new_array, data_);
  delete[] new_array;
}

template <typename T> void ArrayList<T>::ensure_capacity() {
  if (size_ < capacity_)
    return;
  capacity_ = capacity_ == 0 ? 1 : (capacity_ + (capacity_ >> 1));
  T* new_array = copy_array();
  std::swap(new_array, data_);
  delete[] new_array;
}

template <typename T> T* ArrayList<T>::copy_array() {
  T* new_array = new T[capacity_];
  for (std::size_t pos = 0; pos < size_; ++pos)
    new_array[pos] = data_[pos];
  return new_array;
}

template <typename T> std::size_t ArrayList<T>::size() const noexcept {
  return size_;
}

template <typename T> bool ArrayList<T>::empty() const noexcept {
  return size_ == 0;
}

template <typename T> std::size_t ArrayList<T>::capacity() const noexcept {
  return capacity_;
}
