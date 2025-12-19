#pragma once

#include <cstddef>
#include <iostream>
#include <utility>

template <typename T> class LinkedList {
public:
  LinkedList();
  LinkedList(const LinkedList& other);
  LinkedList(LinkedList&& other) noexcept;
  ~LinkedList();

  LinkedList& operator=(const LinkedList& other);
  LinkedList& operator=(LinkedList&& other) noexcept;

  std::size_t size() const noexcept;
  bool empty() const noexcept;
  std::size_t append(T element);
  std::size_t insert(T element, std::size_t index);
  std::size_t remove(std::size_t index);

  T& operator[](std::size_t index);
  const T& operator[](std::size_t index) const;
  friend std::ostream& operator<<(std::ostream& os, const LinkedList<T>& list) {
    if (list.size_ == 0)
      return os << "[]";

    Node* it = list.sentinel_->next;
    os << '[';
    for (std::size_t pos = 1; pos < list.size_; ++pos) {
      os << it->data << ", ";
      it = it->next;
    }
    os << it->data << ']';
    return os;
  }

private:
  struct Node {
    T data;
    Node* next;

    Node() : data(T{}), next(nullptr) {}
    Node(const T& value, Node* n) : data(value), next(n) {}
  };
  Node* sentinel_;
  Node* end_;
  std::size_t size_;

  Node* node_before(std::size_t index) const;
};

#include "linked_list.tpp"
