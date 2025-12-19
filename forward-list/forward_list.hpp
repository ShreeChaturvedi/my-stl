#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

template <typename T> class ForwardList {
public:
  class iterator;
  class const_iterator;

  ForwardList() noexcept;
  ForwardList(const ForwardList& other);
  ForwardList(ForwardList&& other) noexcept;
  ~ForwardList();

  ForwardList& operator=(const ForwardList& other);
  ForwardList& operator=(ForwardList&& other) noexcept;

  bool empty() const noexcept;
  void clear() noexcept;

  T& front();
  const T& front() const;

  void push_front(const T& value);
  void push_front(T&& value);

  template <typename... Args> T& emplace_front(Args&&... args);

  iterator before_begin() noexcept;
  const_iterator before_begin() const noexcept;

  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  const_iterator cbegin() const noexcept;

  iterator end() noexcept;
  const_iterator end() const noexcept;
  const_iterator cend() const noexcept;

  iterator erase_after(iterator pos);
  iterator insert_after(iterator pos, const T& value);
  iterator insert_after(iterator pos, T&& value);

  template <typename... Args> iterator emplace_after(iterator pos, Args&&... args);

private:
  struct NodeBase {
    NodeBase* next = nullptr;
  };

  struct Node final : NodeBase {
    template <typename... Args>
    explicit Node(Args&&... args) : value(std::forward<Args>(args)...) {}
    T value;
  };

  NodeBase head_;

  static Node* as_node(NodeBase* p) {
    return static_cast<Node*>(p);
  }
  static const Node* as_node(const NodeBase* p) {
    return static_cast<const Node*>(p);
  }

  void steal(ForwardList&& other) noexcept;
};

template <typename T> class ForwardList<T>::iterator {
public:
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = T*;
  using reference = T&;
  using iterator_category = std::forward_iterator_tag;

  iterator() noexcept : node_(nullptr) {}

  reference operator*() const {
    return ForwardList::as_node(node_)->value;
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

  bool operator==(const iterator& other) const {
    return node_ == other.node_;
  }
  bool operator!=(const iterator& other) const {
    return node_ != other.node_;
  }

private:
  friend class ForwardList;
  friend class const_iterator;

  explicit iterator(NodeBase* node) noexcept : node_(node) {}
  NodeBase* node_;
};

template <typename T> class ForwardList<T>::const_iterator {
public:
  using value_type = const T;
  using difference_type = std::ptrdiff_t;
  using pointer = const T*;
  using reference = const T&;
  using iterator_category = std::forward_iterator_tag;

  const_iterator() noexcept : node_(nullptr) {}
  const_iterator(iterator it) noexcept : node_(it.node_) {}

  reference operator*() const {
    return ForwardList::as_node(node_)->value;
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

  bool operator==(const const_iterator& other) const {
    return node_ == other.node_;
  }
  bool operator!=(const const_iterator& other) const {
    return node_ != other.node_;
  }

private:
  friend class ForwardList;
  explicit const_iterator(const NodeBase* node) noexcept : node_(node) {}
  const NodeBase* node_;
};

#include "forward_list.tpp"
