#pragma once

#include "unique-ptr/unique_ptr.hpp"
#include "vector/vector.hpp"

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>

template <typename T> class StableVector {
public:
  using value_type = T;
  using size_type = std::size_t;
  using reference = T&;
  using const_reference = const T&;

  class iterator;
  class const_iterator;

  StableVector() = default;
  StableVector(std::initializer_list<T> list);
  StableVector(const StableVector& other);
  StableVector(StableVector&& other) noexcept = default;

  StableVector& operator=(const StableVector& other);
  StableVector& operator=(StableVector&& other) noexcept = default;

  size_type size() const noexcept {
    return slots_.size();
  }
  bool empty() const noexcept {
    return slots_.empty();
  }
  void clear() noexcept {
    slots_.clear();
  }
  void reserve(size_type n) {
    slots_.reserve(n);
  }

  reference operator[](size_type i) noexcept {
    return *slots_[i];
  }
  const_reference operator[](size_type i) const noexcept {
    return *slots_[i];
  }
  reference at(size_type i);
  const_reference at(size_type i) const;

  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;

  template <typename... Args> reference emplace_back(Args&&... args);

  void push_back(const T& value) {
    emplace_back(value);
  }
  void push_back(T&& value) {
    emplace_back(std::move(value));
  }
  void pop_back();

  template <typename... Args> iterator emplace(const_iterator pos, Args&&... args);

  iterator insert(const_iterator pos, const T& value) {
    return emplace(pos, value);
  }
  iterator insert(const_iterator pos, T&& value) {
    return emplace(pos, std::move(value));
  }

  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);

  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  const_iterator cbegin() const noexcept;
  iterator end() noexcept;
  const_iterator end() const noexcept;
  const_iterator cend() const noexcept;

  class iterator final {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    iterator() = default;
    explicit iterator(unique_ptr<T>* p) : p_(p) {}

    reference operator*() const {
      return **p_;
    }
    pointer operator->() const {
      return p_->get();
    }

    iterator& operator++() {
      ++p_;
      return *this;
    }
    iterator operator++(int) {
      iterator tmp(*this);
      ++(*this);
      return tmp;
    }
    iterator& operator--() {
      --p_;
      return *this;
    }
    iterator operator--(int) {
      iterator tmp(*this);
      --(*this);
      return tmp;
    }

    iterator& operator+=(difference_type n) {
      p_ += n;
      return *this;
    }
    iterator& operator-=(difference_type n) {
      p_ -= n;
      return *this;
    }

    friend iterator operator+(iterator it, difference_type n) {
      return iterator(it.p_ + n);
    }
    friend iterator operator+(difference_type n, iterator it) {
      return iterator(it.p_ + n);
    }
    friend iterator operator-(iterator it, difference_type n) {
      return iterator(it.p_ - n);
    }
    friend difference_type operator-(const iterator& a, const iterator& b) {
      return a.p_ - b.p_;
    }

    friend bool operator==(const iterator& a, const iterator& b) {
      return a.p_ == b.p_;
    }
    friend bool operator!=(const iterator& a, const iterator& b) {
      return a.p_ != b.p_;
    }
    friend bool operator<(const iterator& a, const iterator& b) {
      return a.p_ < b.p_;
    }
    friend bool operator<=(const iterator& a, const iterator& b) {
      return a.p_ <= b.p_;
    }
    friend bool operator>(const iterator& a, const iterator& b) {
      return a.p_ > b.p_;
    }
    friend bool operator>=(const iterator& a, const iterator& b) {
      return a.p_ >= b.p_;
    }

    unique_ptr<T>* base() const {
      return p_;
    }

  private:
    unique_ptr<T>* p_ = nullptr;
  };

  class const_iterator final {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = const T;
    using pointer = const T*;
    using reference = const T&;

    const_iterator() = default;
    explicit const_iterator(const unique_ptr<T>* p) : p_(p) {}
    const_iterator(iterator it) : p_(it.base()) {}

    reference operator*() const {
      return **p_;
    }
    pointer operator->() const {
      return p_->get();
    }

    const_iterator& operator++() {
      ++p_;
      return *this;
    }
    const_iterator operator++(int) {
      const_iterator tmp(*this);
      ++(*this);
      return tmp;
    }
    const_iterator& operator--() {
      --p_;
      return *this;
    }
    const_iterator operator--(int) {
      const_iterator tmp(*this);
      --(*this);
      return tmp;
    }

    const_iterator& operator+=(difference_type n) {
      p_ += n;
      return *this;
    }
    const_iterator& operator-=(difference_type n) {
      p_ -= n;
      return *this;
    }

    friend const_iterator operator+(const_iterator it, difference_type n) {
      return const_iterator(it.p_ + n);
    }
    friend const_iterator operator+(difference_type n, const_iterator it) {
      return const_iterator(it.p_ + n);
    }
    friend const_iterator operator-(const_iterator it, difference_type n) {
      return const_iterator(it.p_ - n);
    }
    friend difference_type operator-(const const_iterator& a, const const_iterator& b) {
      return a.p_ - b.p_;
    }

    friend bool operator==(const const_iterator& a, const const_iterator& b) {
      return a.p_ == b.p_;
    }
    friend bool operator!=(const const_iterator& a, const const_iterator& b) {
      return a.p_ != b.p_;
    }
    friend bool operator<(const const_iterator& a, const const_iterator& b) {
      return a.p_ < b.p_;
    }
    friend bool operator<=(const const_iterator& a, const const_iterator& b) {
      return a.p_ <= b.p_;
    }
    friend bool operator>(const const_iterator& a, const const_iterator& b) {
      return a.p_ > b.p_;
    }
    friend bool operator>=(const const_iterator& a, const const_iterator& b) {
      return a.p_ >= b.p_;
    }

    const unique_ptr<T>* base() const {
      return p_;
    }

  private:
    const unique_ptr<T>* p_ = nullptr;
  };

private:
  template <typename... Args> static unique_ptr<T> make(Args&&... args) {
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
  }

  Vector<unique_ptr<T>> slots_;
};

#include "stable_vector.tpp"
