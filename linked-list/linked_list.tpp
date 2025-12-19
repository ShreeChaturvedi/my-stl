#include "linked_list.hpp"

#include <cstddef>
#include <stdexcept>

template <typename T>
LinkedList<T>::LinkedList() : sentinel_(new Node), end_(sentinel_), size_(0) {}

template <typename T> LinkedList<T>::LinkedList(const LinkedList& other) : LinkedList() {
  Node* it = other.sentinel_->next;
  while (it) {
    append(it->data);
    it = it->next;
  }
}

template <typename T>
LinkedList<T>::LinkedList(LinkedList&& other) noexcept
    : sentinel_(std::exchange(other.sentinel_, nullptr)), end_(std::exchange(other.end_, nullptr)),
      size_(std::exchange(other.size_, 0)) {}

template <typename T> LinkedList<T>::~LinkedList() {
  if (!sentinel_)
    return;
  Node* it = sentinel_;
  while (it) {
    Node* nextNode = it->next;
    delete it;
    it = nextNode;
  }
}

template <typename T> LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other) {
  if (this == &other)
    return *this;
  LinkedList tmp(other);
  *this = std::move(tmp);
  return *this;
}

template <typename T> LinkedList<T>& LinkedList<T>::operator=(LinkedList&& other) noexcept {
  if (this == &other)
    return *this;
  if (sentinel_) {
    Node* it = sentinel_;
    while (it) {
      Node* nextNode = it->next;
      delete it;
      it = nextNode;
    }
  }
  sentinel_ = std::exchange(other.sentinel_, nullptr);
  end_ = std::exchange(other.end_, nullptr);
  size_ = std::exchange(other.size_, 0);
  return *this;
}

template <typename T> std::size_t LinkedList<T>::size() const noexcept {
  return size_;
}

template <typename T> bool LinkedList<T>::empty() const noexcept {
  return size_ == 0;
}

template <typename T> std::size_t LinkedList<T>::append(T element) {
  end_->next = new Node(element, nullptr);
  end_ = end_->next;
  return ++size_;
}

template <typename T> std::size_t LinkedList<T>::insert(T element, std::size_t index) {
  if (index > size_)
    throw std::invalid_argument("Supplied index out of bounds");

  Node* prev = node_before(index);
  Node* next = prev->next;
  prev->next = new Node(element, next);
  if (next == nullptr)
    end_ = prev->next;
  return ++size_;
}

template <typename T> std::size_t LinkedList<T>::remove(std::size_t index) {
  if (index >= size_)
    throw std::invalid_argument("Supplied index out of bounds");

  Node* prev = node_before(index);
  Node* victim = prev->next;
  prev->next = victim->next;
  if (victim == end_)
    end_ = prev;
  delete victim;
  return --size_;
}

template <typename T> T& LinkedList<T>::operator[](std::size_t index) {
  return const_cast<T&>(std::as_const(*this)[index]);
}

template <typename T> const T& LinkedList<T>::operator[](std::size_t index) const {
  if (index >= size_)
    throw std::invalid_argument("Supplied index out of bounds");
  Node* it = sentinel_->next;
  for (std::size_t pos = 0; pos < index; ++pos)
    it = it->next;
  return it->data;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::node_before(std::size_t index) const {
  if (index > size_)
    throw std::invalid_argument("Supplied index out of bounds");
  if (index == size_)
    return end_;

  Node* result = sentinel_;
  for (std::size_t pos = 0; pos < index; ++pos)
    result = result->next;
  return result;
}
