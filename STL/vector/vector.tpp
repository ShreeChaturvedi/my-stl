#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

template <typename T>
Vector<T>::Vector() noexcept : size_(0), capacity_(0), data_(nullptr) {}

template <typename T>
Vector<T>::Vector(std::size_t initial_capacity) : Vector() {
  reserve(initial_capacity);
}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> list) : Vector(list.size()) {
  for (const auto& v : list) emplace_back(v);
}

template <typename T>
Vector<T>::Vector(const Vector& other) : Vector(other.size_) {
  for (const auto& v : other) emplace_back(v);
}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept
    : size_(std::exchange(other.size_, 0)),
      capacity_(std::exchange(other.capacity_, 0)),
      data_(std::exchange(other.data_, nullptr)) {}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
  if (this == &other) return *this;
  Vector tmp(other);
  swap(tmp);
  return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
  if (this == &other) return *this;
  clear();
  deallocate();
  size_ = std::exchange(other.size_, 0);
  capacity_ = std::exchange(other.capacity_, 0);
  data_ = std::exchange(other.data_, nullptr);
  return *this;
}

template <typename T>
Vector<T>::~Vector() {
  clear();
  deallocate();
}

template <typename T>
void Vector<T>::swap(Vector& other) noexcept {
  using std::swap;
  swap(size_, other.size_);
  swap(capacity_, other.capacity_);
  swap(data_, other.data_);
}

template <typename T>
T& Vector<T>::operator[](std::size_t pos) noexcept {
  return data_[pos];
}

template <typename T>
const T& Vector<T>::operator[](std::size_t pos) const noexcept {
  return data_[pos];
}

template <typename T>
T& Vector<T>::at(std::size_t pos) {
  if (pos >= size_) throw std::out_of_range("Vector::at out of range");
  return data_[pos];
}

template <typename T>
const T& Vector<T>::at(std::size_t pos) const {
  if (pos >= size_) throw std::out_of_range("Vector::at out of range");
  return data_[pos];
}

template <typename T>
std::size_t Vector<T>::size() const noexcept {
  return size_;
}

template <typename T>
bool Vector<T>::empty() const noexcept {
  return size_ == 0;
}

template <typename T>
std::size_t Vector<T>::capacity() const noexcept {
  return capacity_;
}

template <typename T>
T* Vector<T>::data() noexcept {
  return data_;
}

template <typename T>
const T* Vector<T>::data() const noexcept {
  return data_;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::begin() noexcept {
  return data_;
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::begin() const noexcept {
  return data_;
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::cbegin() const noexcept {
  return data_;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::end() noexcept {
  return data_ + size_;
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::end() const noexcept {
  return data_ + size_;
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::cend() const noexcept {
  return data_ + size_;
}

template <typename T>
void Vector<T>::clear() noexcept {
  std::destroy_n(data_, size_);
  size_ = 0;
}

template <typename T>
void Vector<T>::reserve(std::size_t new_capacity) {
  if (new_capacity <= capacity_) return;

  T* new_data = allocate(new_capacity);
  if constexpr (std::is_nothrow_move_constructible_v<T>) {
    std::uninitialized_move_n(data_, size_, new_data);
  } else {
    std::uninitialized_copy_n(data_, size_, new_data);
  }
  std::destroy_n(data_, size_);
  deallocate();
  data_ = new_data;
  capacity_ = new_capacity;
}

template <typename T>
void Vector<T>::push_back(const T& element) {
  emplace_back(element);
}

template <typename T>
void Vector<T>::push_back(T&& element) {
  emplace_back(std::move(element));
}

template <typename T>
template <typename... Args>
T& Vector<T>::emplace_back(Args&&... args) {
  ensure_capacity_for_one_more();
  T* slot = data_ + size_;
  std::construct_at(slot, std::forward<Args>(args)...);
  ++size_;
  return *slot;
}

template <typename T>
void Vector<T>::ensure_capacity_for_one_more() {
  if (size_ < capacity_) return;
  const std::size_t next = capacity_ == 0 ? 1 : (capacity_ == 1 ? 2 : (capacity_ + (capacity_ >> 1)));
  reserve(next);
}

template <typename T>
T* Vector<T>::allocate(std::size_t n) {
  return std::allocator<T>{}.allocate(n);
}

template <typename T>
void Vector<T>::deallocate() noexcept {
  if (!data_) return;
  std::allocator<T>{}.deallocate(data_, capacity_);
  data_ = nullptr;
  capacity_ = 0;
}
