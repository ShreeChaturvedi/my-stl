#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

template <typename T> class List {
public:
  class iterator;
  class const_iterator;

  List() noexcept;
  List(const List& other);
  List(List&& other) noexcept;
  ~List();

  List& operator=(const List& other);
  List& operator=(List&& other) noexcept;

  bool empty() const noexcept;
  std::size_t size() const noexcept;
  void clear() noexcept;

  T& front();
  const T& front() const;

  T& back();
  const T& back() const;

  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  const_iterator cbegin() const noexcept;

  iterator end() noexcept;
  const_iterator end() const noexcept;
  const_iterator cend() const noexcept;

  iterator push_front(const T& value);
  iterator push_front(T&& value);

  iterator push_back(const T& value);
  iterator push_back(T&& value);

  void pop_front();
  void pop_back();

  iterator erase(iterator pos);
  void move_to_front(iterator pos);

private:
  struct NodeBase {
    NodeBase* prev = nullptr;
    NodeBase* next = nullptr;
  };

  struct Node final : NodeBase {
    template <typename... Args>
    explicit Node(Args&&... args) : value(std::forward<Args>(args)...) {}
    T value;
  };

  NodeBase sentinel_;
  std::size_t size_;

  static Node* as_node(NodeBase* p) {
    return static_cast<Node*>(p);
  }
  static const Node* as_node(const NodeBase* p) {
    return static_cast<const Node*>(p);
  }

  static void link_between(NodeBase* node, NodeBase* before, NodeBase* after) noexcept;
  static void unlink(NodeBase* node) noexcept;
  void steal(List&& other) noexcept;
};

template <typename T> class List<T>::iterator {
public:
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = T*;
  using reference = T&;
  using iterator_category = std::bidirectional_iterator_tag;

  iterator() noexcept : node_(nullptr) {}

  reference operator*() const {
    return List::as_node(node_)->value;
  }
  pointer operator->() const {
    return std::addressof(operator*());
  }

  iterator& operator++() {
    node_ = node_->next;
    return *this;
  }

  iterator operator++(int) {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

  iterator& operator--() {
    node_ = node_->prev;
    return *this;
  }

  iterator operator--(int) {
    auto tmp = *this;
    --(*this);
    return tmp;
  }

  bool operator==(const iterator& other) const {
    return node_ == other.node_;
  }
  bool operator!=(const iterator& other) const {
    return node_ != other.node_;
  }

private:
  friend class List;
  friend class const_iterator;
  explicit iterator(NodeBase* node) noexcept : node_(node) {}
  NodeBase* node_;
};

template <typename T> class List<T>::const_iterator {
public:
  using value_type = const T;
  using difference_type = std::ptrdiff_t;
  using pointer = const T*;
  using reference = const T&;
  using iterator_category = std::bidirectional_iterator_tag;

  const_iterator() noexcept : node_(nullptr) {}
  const_iterator(iterator it) noexcept : node_(it.node_) {}

  reference operator*() const {
    return List::as_node(node_)->value;
  }
  pointer operator->() const {
    return std::addressof(operator*());
  }

  const_iterator& operator++() {
    node_ = node_->next;
    return *this;
  }

  const_iterator operator++(int) {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

  const_iterator& operator--() {
    node_ = node_->prev;
    return *this;
  }

  const_iterator operator--(int) {
    auto tmp = *this;
    --(*this);
    return tmp;
  }

  bool operator==(const const_iterator& other) const {
    return node_ == other.node_;
  }
  bool operator!=(const const_iterator& other) const {
    return node_ != other.node_;
  }

private:
  friend class List;
  explicit const_iterator(const NodeBase* node) noexcept : node_(node) {}
  const NodeBase* node_;
};

#include "list.tpp"
