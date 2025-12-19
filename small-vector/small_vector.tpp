#include <algorithm>
#include <cstring>
#include <new>

template <typename T, std::size_t InlineCapacity>
SmallVector<T, InlineCapacity>::SmallVector() noexcept
    : size_(0), capacity_(inline_capacity()),
      data_(inline_capacity() == 0 ? nullptr : inline_data()) {}

template <typename T, std::size_t InlineCapacity>
SmallVector<T, InlineCapacity>::SmallVector(std::initializer_list<T> list) : SmallVector() {
  reserve(list.size());
  for (const auto& v : list)
    emplace_back(v);
}

template <typename T, std::size_t InlineCapacity>
SmallVector<T, InlineCapacity>::SmallVector(const SmallVector& other) : SmallVector() {
  reserve(other.size_);
  for (const auto& v : other)
    emplace_back(v);
}

template <typename T, std::size_t InlineCapacity>
SmallVector<T, InlineCapacity>::SmallVector(SmallVector&& other) noexcept(
    std::is_nothrow_move_constructible_v<T>)
    : SmallVector() {
  if (!other.using_inline_storage()) {
    data_ =
        std::exchange(other.data_, other.inline_capacity() == 0 ? nullptr : other.inline_data());
    size_ = std::exchange(other.size_, 0);
    capacity_ = std::exchange(other.capacity_, other.inline_capacity());
    return;
  }

  reserve(other.size_);
  for (auto& v : other)
    emplace_back(std::move(v));
  other.clear();
}

template <typename T, std::size_t InlineCapacity>
SmallVector<T, InlineCapacity>&
SmallVector<T, InlineCapacity>::operator=(const SmallVector& other) {
  if (this == &other)
    return *this;
  SmallVector tmp(other);
  swap(tmp);
  return *this;
}

template <typename T, std::size_t InlineCapacity>
SmallVector<T, InlineCapacity>& SmallVector<T, InlineCapacity>::operator=(
    SmallVector&& other) noexcept(std::is_nothrow_move_constructible_v<T>) {
  if (this == &other)
    return *this;

  clear();
  if (!using_inline_storage())
    deallocate(data_, capacity_);

  data_ = inline_capacity() == 0 ? nullptr : inline_data();
  size_ = 0;
  capacity_ = inline_capacity();

  if (!other.using_inline_storage()) {
    data_ =
        std::exchange(other.data_, other.inline_capacity() == 0 ? nullptr : other.inline_data());
    size_ = std::exchange(other.size_, 0);
    capacity_ = std::exchange(other.capacity_, other.inline_capacity());
    return *this;
  }

  reserve(other.size_);
  for (auto& v : other)
    emplace_back(std::move(v));
  other.clear();
  return *this;
}

template <typename T, std::size_t InlineCapacity> SmallVector<T, InlineCapacity>::~SmallVector() {
  clear();
  if (!using_inline_storage())
    deallocate(data_, capacity_);
}

template <typename T, std::size_t InlineCapacity>
T* SmallVector<T, InlineCapacity>::inline_data() noexcept {
  return std::launder(reinterpret_cast<T*>(inline_storage_));
}

template <typename T, std::size_t InlineCapacity>
const T* SmallVector<T, InlineCapacity>::inline_data() const noexcept {
  return std::launder(reinterpret_cast<const T*>(inline_storage_));
}

template <typename T, std::size_t InlineCapacity>
T* SmallVector<T, InlineCapacity>::allocate(size_type n) {
  if (n == 0)
    return nullptr;
  return std::allocator<T>{}.allocate(n);
}

template <typename T, std::size_t InlineCapacity>
void SmallVector<T, InlineCapacity>::deallocate(T* p, size_type n) noexcept {
  if (!p || n == 0)
    return;
  std::allocator<T>{}.deallocate(p, n);
}

template <typename T, std::size_t InlineCapacity>
T& SmallVector<T, InlineCapacity>::at(size_type i) {
  if (i >= size_)
    throw std::out_of_range("SmallVector::at out of range");
  return data_[i];
}

template <typename T, std::size_t InlineCapacity>
const T& SmallVector<T, InlineCapacity>::at(size_type i) const {
  if (i >= size_)
    throw std::out_of_range("SmallVector::at out of range");
  return data_[i];
}

template <typename T, std::size_t InlineCapacity> T& SmallVector<T, InlineCapacity>::front() {
  if (empty())
    throw std::out_of_range("SmallVector::front on empty");
  return data_[0];
}

template <typename T, std::size_t InlineCapacity>
const T& SmallVector<T, InlineCapacity>::front() const {
  if (empty())
    throw std::out_of_range("SmallVector::front on empty");
  return data_[0];
}

template <typename T, std::size_t InlineCapacity> T& SmallVector<T, InlineCapacity>::back() {
  if (empty())
    throw std::out_of_range("SmallVector::back on empty");
  return data_[size_ - 1];
}

template <typename T, std::size_t InlineCapacity>
const T& SmallVector<T, InlineCapacity>::back() const {
  if (empty())
    throw std::out_of_range("SmallVector::back on empty");
  return data_[size_ - 1];
}

template <typename T, std::size_t InlineCapacity>
void SmallVector<T, InlineCapacity>::clear() noexcept {
  std::destroy_n(data_, size_);
  size_ = 0;
}

template <typename T, std::size_t InlineCapacity>
void SmallVector<T, InlineCapacity>::grow_to(size_type new_capacity) {
  T* new_data = allocate(new_capacity);
  if constexpr (std::is_trivially_copyable_v<T> && std::is_trivially_copy_assignable_v<T>) {
    if (size_ > 0)
      std::memcpy(new_data, data_, size_ * sizeof(T));
  } else if constexpr (std::is_nothrow_move_constructible_v<T>) {
    std::uninitialized_move_n(data_, size_, new_data);
  } else {
    std::uninitialized_copy_n(data_, size_, new_data);
  }
  std::destroy_n(data_, size_);
  if (!using_inline_storage())
    deallocate(data_, capacity_);
  data_ = new_data;
  capacity_ = new_capacity;
}

template <typename T, std::size_t InlineCapacity>
void SmallVector<T, InlineCapacity>::reserve(size_type new_capacity) {
  if (new_capacity <= capacity_)
    return;
  grow_to(new_capacity);
}

template <typename T, std::size_t InlineCapacity>
template <typename... Args>
T& SmallVector<T, InlineCapacity>::emplace_back(Args&&... args) {
  if (size_ == capacity_) {
    const size_type required = size_ + 1;
    size_type next = capacity_ == 0 ? 1 : (capacity_ * 2);
    if (next < required)
      next = required;
    grow_to(next);
  }

  T* slot = data_ + size_;
  std::construct_at(slot, std::forward<Args>(args)...);
  ++size_;
  return *slot;
}

template <typename T, std::size_t InlineCapacity> void SmallVector<T, InlineCapacity>::pop_back() {
  if (empty())
    throw std::out_of_range("SmallVector::pop_back on empty");
  std::destroy_at(data_ + (size_ - 1));
  --size_;
}

template <typename T, std::size_t InlineCapacity>
template <typename... Args>
typename SmallVector<T, InlineCapacity>::iterator
SmallVector<T, InlineCapacity>::emplace(const_iterator pos, Args&&... args) {
  const size_type index = static_cast<size_type>(pos - cbegin());
  if (index > size_)
    throw std::out_of_range("SmallVector::emplace position out of range");

  if (index == size_) {
    emplace_back(std::forward<Args>(args)...);
    return data_ + index;
  }

  if constexpr (std::is_move_assignable_v<T> || std::is_copy_assignable_v<T>) {
    if (size_ == capacity_) {
      const size_type required = size_ + 1;
      size_type next = capacity_ == 0 ? 1 : (capacity_ * 2);
      if (next < required)
        next = required;
      grow_to(next);
    }

    const size_type old_size = size_;
    if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
      std::construct_at(data_ + old_size, std::move(data_[old_size - 1]));
    } else {
      std::construct_at(data_ + old_size, data_[old_size - 1]);
    }

    if constexpr (std::is_nothrow_move_assignable_v<T> || !std::is_copy_assignable_v<T>) {
      for (size_type i = old_size - 1; i > index; --i)
        data_[i] = std::move(data_[i - 1]);
    } else {
      for (size_type i = old_size - 1; i > index; --i)
        data_[i] = data_[i - 1];
    }

    std::destroy_at(data_ + index);
    std::construct_at(data_ + index, std::forward<Args>(args)...);
    ++size_;
    return data_ + index;
  } else {
    const size_type required = size_ + 1;
    size_type new_capacity = capacity_;
    if (new_capacity < required) {
      new_capacity = capacity_ == 0 ? 1 : (capacity_ == 1 ? 2 : (capacity_ + (capacity_ >> 1)));
      if (new_capacity < required)
        new_capacity = required;
    }

    T* new_data = allocate(new_capacity);
    size_type constructed = 0;
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
      deallocate(new_data, new_capacity);
      throw;
    }

    std::destroy_n(data_, size_);
    if (!using_inline_storage())
      deallocate(data_, capacity_);
    data_ = new_data;
    capacity_ = new_capacity;
    ++size_;
    return data_ + index;
  }
}

template <typename T, std::size_t InlineCapacity>
typename SmallVector<T, InlineCapacity>::iterator
SmallVector<T, InlineCapacity>::erase(const_iterator pos) {
  const size_type index = static_cast<size_type>(pos - cbegin());
  if (index >= size_)
    throw std::out_of_range("SmallVector::erase position out of range");
  return erase(cbegin() + index, cbegin() + index + 1);
}

template <typename T, std::size_t InlineCapacity>
typename SmallVector<T, InlineCapacity>::iterator
SmallVector<T, InlineCapacity>::erase(const_iterator first, const_iterator last) {
  const size_type first_index = static_cast<size_type>(first - cbegin());
  const size_type last_index = static_cast<size_type>(last - cbegin());
  if (first_index > last_index)
    throw std::out_of_range("SmallVector::erase invalid range");
  if (last_index > size_)
    throw std::out_of_range("SmallVector::erase range out of range");

  const size_type count = last_index - first_index;
  if (count == 0)
    return data_ + first_index;

  if constexpr (std::is_move_assignable_v<T> || std::is_copy_assignable_v<T>) {
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
    size_type constructed = 0;
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
      deallocate(new_data, capacity_);
      throw;
    }

    std::destroy_n(data_, size_);
    if (!using_inline_storage())
      deallocate(data_, capacity_);
    data_ = new_data;
    size_ -= count;
    return data_ + first_index;
  }
}

template <typename T, std::size_t InlineCapacity>
void SmallVector<T, InlineCapacity>::swap(SmallVector& other) noexcept(
    std::is_nothrow_move_constructible_v<T>) {
  if (!using_inline_storage() && !other.using_inline_storage()) {
    using std::swap;
    swap(size_, other.size_);
    swap(capacity_, other.capacity_);
    swap(data_, other.data_);
    return;
  }

  SmallVector tmp(std::move(*this));
  *this = std::move(other);
  other = std::move(tmp);
}
