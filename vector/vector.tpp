#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

template <typename T> Vector<T>::Vector() noexcept : size_(0), capacity_(0), data_(nullptr) {}

template <typename T> Vector<T>::Vector(std::size_t initial_capacity) : Vector() {
  reserve(initial_capacity);
}

template <typename T> Vector<T>::Vector(std::initializer_list<T> list) : Vector(list.size()) {
  for (const auto& v : list)
    emplace_back(v);
}

template <typename T> Vector<T>::Vector(const Vector& other) : Vector(other.size_) {
  for (const auto& v : other)
    emplace_back(v);
}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept
    : size_(std::exchange(other.size_, 0)), capacity_(std::exchange(other.capacity_, 0)),
      data_(std::exchange(other.data_, nullptr)) {}

template <typename T> Vector<T>& Vector<T>::operator=(const Vector& other) {
  if (this == &other)
    return *this;
  Vector tmp(other);
  swap(tmp);
  return *this;
}

template <typename T> Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
  if (this == &other)
    return *this;
  clear();
  deallocate();
  size_ = std::exchange(other.size_, 0);
  capacity_ = std::exchange(other.capacity_, 0);
  data_ = std::exchange(other.data_, nullptr);
  return *this;
}

template <typename T> Vector<T>::~Vector() {
  clear();
  deallocate();
}

template <typename T> void Vector<T>::swap(Vector& other) noexcept {
  using std::swap;
  swap(size_, other.size_);
  swap(capacity_, other.capacity_);
  swap(data_, other.data_);
}

template <typename T> T& Vector<T>::operator[](std::size_t pos) noexcept {
  return data_[pos];
}

template <typename T> const T& Vector<T>::operator[](std::size_t pos) const noexcept {
  return data_[pos];
}

template <typename T> T& Vector<T>::at(std::size_t pos) {
  if (pos >= size_)
    throw std::out_of_range("Vector::at out of range");
  return data_[pos];
}

template <typename T> const T& Vector<T>::at(std::size_t pos) const {
  if (pos >= size_)
    throw std::out_of_range("Vector::at out of range");
  return data_[pos];
}

template <typename T> std::size_t Vector<T>::size() const noexcept {
  return size_;
}

template <typename T> bool Vector<T>::empty() const noexcept {
  return size_ == 0;
}

template <typename T> std::size_t Vector<T>::capacity() const noexcept {
  return capacity_;
}

template <typename T> T* Vector<T>::data() noexcept {
  return data_;
}

template <typename T> const T* Vector<T>::data() const noexcept {
  return data_;
}

template <typename T> typename Vector<T>::iterator Vector<T>::begin() noexcept {
  return data_;
}

template <typename T> typename Vector<T>::const_iterator Vector<T>::begin() const noexcept {
  return data_;
}

template <typename T> typename Vector<T>::const_iterator Vector<T>::cbegin() const noexcept {
  return data_;
}

template <typename T> typename Vector<T>::iterator Vector<T>::end() noexcept {
  return data_ + size_;
}

template <typename T> typename Vector<T>::const_iterator Vector<T>::end() const noexcept {
  return data_ + size_;
}

template <typename T> typename Vector<T>::const_iterator Vector<T>::cend() const noexcept {
  return data_ + size_;
}

template <typename T> typename Vector<T>::reverse_iterator Vector<T>::rbegin() noexcept {
  return reverse_iterator(end());
}

template <typename T>
typename Vector<T>::const_reverse_iterator Vector<T>::rbegin() const noexcept {
  return const_reverse_iterator(end());
}

template <typename T>
typename Vector<T>::const_reverse_iterator Vector<T>::crbegin() const noexcept {
  return const_reverse_iterator(end());
}

template <typename T> typename Vector<T>::reverse_iterator Vector<T>::rend() noexcept {
  return reverse_iterator(begin());
}

template <typename T> typename Vector<T>::const_reverse_iterator Vector<T>::rend() const noexcept {
  return const_reverse_iterator(begin());
}

template <typename T> typename Vector<T>::const_reverse_iterator Vector<T>::crend() const noexcept {
  return const_reverse_iterator(begin());
}

template <typename T> void Vector<T>::clear() noexcept {
  std::destroy_n(data_, size_);
  size_ = 0;
}

template <typename T> void Vector<T>::reserve(std::size_t new_capacity) {
  if (new_capacity <= capacity_)
    return;

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

template <typename T> void Vector<T>::resize(std::size_t new_size) {
  if (new_size < size_) {
    std::destroy_n(data_ + new_size, size_ - new_size);
    size_ = new_size;
    return;
  }

  if (new_size == size_)
    return;

  static_assert(std::is_default_constructible_v<T>,
                "Vector::resize requires default constructible T");
  reserve(new_size);
  while (size_ < new_size) {
    std::construct_at(data_ + size_);
    ++size_;
  }
}

template <typename T> T& Vector<T>::front() {
  if (empty())
    throw std::out_of_range("Vector::front on empty");
  return data_[0];
}

template <typename T> const T& Vector<T>::front() const {
  if (empty())
    throw std::out_of_range("Vector::front on empty");
  return data_[0];
}

template <typename T> T& Vector<T>::back() {
  if (empty())
    throw std::out_of_range("Vector::back on empty");
  return data_[size_ - 1];
}

template <typename T> const T& Vector<T>::back() const {
  if (empty())
    throw std::out_of_range("Vector::back on empty");
  return data_[size_ - 1];
}

template <typename T> void Vector<T>::pop_back() {
  if (empty())
    throw std::out_of_range("Vector::pop_back on empty");
  std::destroy_at(data_ + (size_ - 1));
  --size_;
}

template <typename T> void Vector<T>::push_back(const T& element) {
  emplace_back(element);
}

template <typename T> void Vector<T>::push_back(T&& element) {
  emplace_back(std::move(element));
}

template <typename T> template <typename... Args> T& Vector<T>::emplace_back(Args&&... args) {
  ensure_capacity_for_one_more();
  T* slot = data_ + size_;
  std::construct_at(slot, std::forward<Args>(args)...);
  ++size_;
  return *slot;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, const T& value) {
  return emplace(pos, value);
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, T&& value) {
  return emplace(pos, std::move(value));
}

template <typename T>
template <typename... Args>
typename Vector<T>::iterator Vector<T>::emplace(const_iterator pos, Args&&... args) {
  const std::size_t index = static_cast<std::size_t>(pos - cbegin());
  if (index > size_)
    throw std::out_of_range("Vector::emplace position out of range");

  if (index == size_) {
    emplace_back(std::forward<Args>(args)...);
    return data_ + index;
  }

  if constexpr (std::is_move_assignable_v<T> || std::is_copy_assignable_v<T>) {
    ensure_capacity_for_one_more();
    const std::size_t old_size = size_;
    if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
      std::construct_at(data_ + old_size, std::move(data_[old_size - 1]));
    } else {
      std::construct_at(data_ + old_size, data_[old_size - 1]);
    }

    if constexpr (std::is_nothrow_move_assignable_v<T> || !std::is_copy_assignable_v<T>) {
      for (std::size_t i = old_size - 1; i > index; --i)
        data_[i] = std::move(data_[i - 1]);
    } else {
      for (std::size_t i = old_size - 1; i > index; --i)
        data_[i] = data_[i - 1];
    }

    std::destroy_at(data_ + index);
    std::construct_at(data_ + index, std::forward<Args>(args)...);
    ++size_;
    return data_ + index;
  } else {
    const std::size_t required = size_ + 1;
    std::size_t new_capacity = capacity_;
    if (new_capacity < required) {
      new_capacity = capacity_ == 0 ? 1 : (capacity_ == 1 ? 2 : (capacity_ + (capacity_ >> 1)));
      if (new_capacity < required)
        new_capacity = required;
    }

    T* new_data = allocate(new_capacity);
    std::size_t constructed = 0;
    try {
      if constexpr (std::is_nothrow_move_constructible_v<T>) {
        std::uninitialized_move_n(data_, index, new_data);
      } else {
        std::uninitialized_copy_n(data_, index, new_data);
      }
      constructed += index;

      std::construct_at(new_data + index, std::forward<Args>(args)...);
      ++constructed;

      if constexpr (std::is_nothrow_move_constructible_v<T>) {
        std::uninitialized_move_n(data_ + index, size_ - index, new_data + index + 1);
      } else {
        std::uninitialized_copy_n(data_ + index, size_ - index, new_data + index + 1);
      }
      constructed += (size_ - index);
    } catch (...) {
      std::destroy_n(new_data, constructed);
      std::allocator<T>{}.deallocate(new_data, new_capacity);
      throw;
    }

    std::destroy_n(data_, size_);
    deallocate();
    data_ = new_data;
    capacity_ = new_capacity;
    ++size_;
    return data_ + index;
  }
}

template <typename T> typename Vector<T>::iterator Vector<T>::erase(const_iterator pos) {
  const std::size_t index = static_cast<std::size_t>(pos - cbegin());
  if (index >= size_)
    throw std::out_of_range("Vector::erase position out of range");
  return erase(cbegin() + index, cbegin() + index + 1);
}

template <typename T>
typename Vector<T>::iterator Vector<T>::erase(const_iterator first, const_iterator last) {
  const std::size_t first_index = static_cast<std::size_t>(first - cbegin());
  const std::size_t last_index = static_cast<std::size_t>(last - cbegin());
  if (first_index > last_index)
    throw std::out_of_range("Vector::erase invalid range");
  if (last_index > size_)
    throw std::out_of_range("Vector::erase range out of range");

  const std::size_t count = last_index - first_index;
  if (count == 0)
    return data_ + first_index;

  constexpr bool can_shift_in_place = std::is_move_assignable_v<T> || std::is_copy_assignable_v<T>;
  if constexpr (can_shift_in_place) {
    if constexpr (std::is_nothrow_move_assignable_v<T> || !std::is_copy_assignable_v<T>) {
      std::move(data_ + last_index, data_ + size_, data_ + first_index);
    } else {
      std::copy(data_ + last_index, data_ + size_, data_ + first_index);
    }

    std::destroy_n(data_ + (size_ - count), count);
    size_ -= count;
    return data_ + first_index;
  } else {
    T* new_data = allocate(capacity_);
    std::size_t constructed = 0;
    try {
      if constexpr (std::is_nothrow_move_constructible_v<T>) {
        std::uninitialized_move_n(data_, first_index, new_data);
      } else {
        std::uninitialized_copy_n(data_, first_index, new_data);
      }
      constructed += first_index;

      if constexpr (std::is_nothrow_move_constructible_v<T>) {
        std::uninitialized_move_n(data_ + last_index, size_ - last_index, new_data + first_index);
      } else {
        std::uninitialized_copy_n(data_ + last_index, size_ - last_index, new_data + first_index);
      }
      constructed += (size_ - last_index);
    } catch (...) {
      std::destroy_n(new_data, constructed);
      std::allocator<T>{}.deallocate(new_data, capacity_);
      throw;
    }

    std::destroy_n(data_, size_);
    deallocate();
    data_ = new_data;
    size_ -= count;
    return data_ + first_index;
  }
}

template <typename T> void Vector<T>::ensure_capacity_for_one_more() {
  if (size_ < capacity_)
    return;
  const std::size_t next =
      capacity_ == 0 ? 1 : (capacity_ == 1 ? 2 : (capacity_ + (capacity_ >> 1)));
  reserve(next);
}

template <typename T> T* Vector<T>::allocate(std::size_t n) {
  return std::allocator<T>{}.allocate(n);
}

template <typename T> void Vector<T>::deallocate() noexcept {
  if (!data_)
    return;
  std::allocator<T>{}.deallocate(data_, capacity_);
  data_ = nullptr;
  capacity_ = 0;
}
