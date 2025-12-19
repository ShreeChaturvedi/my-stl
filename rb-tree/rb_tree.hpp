#pragma once

#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

template <typename Value, typename KeyOfValue, typename Compare, bool Multi> class RbTree {
public:
  class iterator;
  class const_iterator;

  using value_type = Value;
  using size_type = std::size_t;

  RbTree() : root_(nullptr), size_(0), comp_(), key_of_() {}
  explicit RbTree(Compare comp) : root_(nullptr), size_(0), comp_(std::move(comp)), key_of_() {}

  RbTree(const RbTree&) = delete;
  RbTree& operator=(const RbTree&) = delete;

  RbTree(RbTree&& other) noexcept
      : root_(nullptr), size_(0), comp_(std::move(other.comp_)), key_of_() {
    root_ = std::exchange(other.root_, nullptr);
    size_ = std::exchange(other.size_, 0);
    blocks_ = std::move(other.blocks_);
    free_ = std::exchange(other.free_, nullptr);
  }

  RbTree& operator=(RbTree&& other) noexcept {
    if (this == &other)
      return *this;
    clear();
    release_blocks();
    root_ = std::exchange(other.root_, nullptr);
    size_ = std::exchange(other.size_, 0);
    comp_ = std::move(other.comp_);
    blocks_ = std::move(other.blocks_);
    free_ = std::exchange(other.free_, nullptr);
    return *this;
  }

  ~RbTree() {
    clear();
    release_blocks();
  }

  bool empty() const noexcept {
    return size_ == 0;
  }
  size_type size() const noexcept {
    return size_;
  }

  iterator begin() noexcept {
    return iterator(minimum(root_), this);
  }
  const_iterator begin() const noexcept {
    return const_iterator(minimum(root_), this);
  }
  const_iterator cbegin() const noexcept {
    return const_iterator(minimum(root_), this);
  }

  iterator end() noexcept {
    return iterator(nullptr, this);
  }
  const_iterator end() const noexcept {
    return const_iterator(nullptr, this);
  }
  const_iterator cend() const noexcept {
    return const_iterator(nullptr, this);
  }

  void clear() noexcept {
    destroy_subtree(root_);
    root_ = nullptr;
    size_ = 0;
  }

  template <typename Key> iterator find(const Key& key) noexcept {
    Node* n = root_;
    while (n) {
      if (comp_(key, key_of_(n->value)))
        n = n->left;
      else if (comp_(key_of_(n->value), key))
        n = n->right;
      else
        return iterator(n, this);
    }
    return end();
  }

  template <typename Key> const_iterator find(const Key& key) const noexcept {
    const Node* n = root_;
    while (n) {
      if (comp_(key, key_of_(n->value)))
        n = n->left;
      else if (comp_(key_of_(n->value), key))
        n = n->right;
      else
        return const_iterator(n, this);
    }
    return cend();
  }

  template <typename Key> iterator lower_bound(const Key& key) noexcept {
    Node* curr = root_;
    Node* result = nullptr;
    while (curr) {
      if (!comp_(key_of_(curr->value), key)) {
        result = curr;
        curr = curr->left;
      } else {
        curr = curr->right;
      }
    }
    return iterator(result, this);
  }

  template <typename Key> iterator upper_bound(const Key& key) noexcept {
    Node* curr = root_;
    Node* result = nullptr;
    while (curr) {
      if (comp_(key, key_of_(curr->value))) {
        result = curr;
        curr = curr->left;
      } else {
        curr = curr->right;
      }
    }
    return iterator(result, this);
  }

  template <typename Key> const_iterator lower_bound(const Key& key) const noexcept {
    const Node* curr = root_;
    const Node* result = nullptr;
    while (curr) {
      if (!comp_(key_of_(curr->value), key)) {
        result = curr;
        curr = curr->left;
      } else {
        curr = curr->right;
      }
    }
    return const_iterator(result, this);
  }

  template <typename Key> const_iterator upper_bound(const Key& key) const noexcept {
    const Node* curr = root_;
    const Node* result = nullptr;
    while (curr) {
      if (comp_(key, key_of_(curr->value))) {
        result = curr;
        curr = curr->left;
      } else {
        curr = curr->right;
      }
    }
    return const_iterator(result, this);
  }

  std::pair<iterator, bool> insert_unique(value_type value) {
    Node* parent = nullptr;
    Node* curr = root_;
    const auto& k = key_of_(value);
    while (curr) {
      parent = curr;
      if (comp_(k, key_of_(curr->value)))
        curr = curr->left;
      else if (comp_(key_of_(curr->value), k))
        curr = curr->right;
      else
        return {iterator(curr, this), false};
    }
    Node* node = create_node(std::move(value));
    node->parent = parent;
    link_node(parent, node, k);
    insert_fixup(node);
    ++size_;
    return {iterator(node, this), true};
  }

  iterator insert_multi(value_type value) {
    Node* parent = nullptr;
    Node* curr = root_;
    const auto& k = key_of_(value);
    while (curr) {
      parent = curr;
      if (comp_(k, key_of_(curr->value)))
        curr = curr->left;
      else
        curr = curr->right;
    }
    Node* node = create_node(std::move(value));
    node->parent = parent;
    link_node(parent, node, k);
    insert_fixup(node);
    ++size_;
    return iterator(node, this);
  }

  iterator erase(iterator pos) {
    Node* z = pos.node_;
    Node* next = successor(z);
    erase_node(z);
    return iterator(next, this);
  }

private:
  enum class Color : unsigned char { Red, Black };

  struct Node {
    explicit Node(value_type v) : value(std::move(v)) {}
    value_type value;
    Node* parent = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;
    Color color = Color::Red;
  };

  struct FreeNode {
    FreeNode* next = nullptr;
  };

  struct Storage {
    alignas(Node) std::byte data[sizeof(Node)];
  };
  static constexpr size_type kBlockSize = 512;

  Node* root_;
  size_type size_;
  Compare comp_;
  KeyOfValue key_of_;
  std::allocator<Storage> storage_alloc_{};
  std::vector<Storage*> blocks_;
  FreeNode* free_ = nullptr;

  static Node* minimum(Node* n) noexcept {
    if (!n)
      return nullptr;
    while (n->left)
      n = n->left;
    return n;
  }

  static const Node* minimum(const Node* n) noexcept {
    if (!n)
      return nullptr;
    while (n->left)
      n = n->left;
    return n;
  }

  static Node* maximum(Node* n) noexcept {
    if (!n)
      return nullptr;
    while (n->right)
      n = n->right;
    return n;
  }

  static Node* successor(Node* n) noexcept {
    if (!n)
      return nullptr;
    if (n->right)
      return minimum(n->right);
    Node* p = n->parent;
    while (p && n == p->right) {
      n = p;
      p = p->parent;
    }
    return p;
  }

  static Node* predecessor(Node* n) noexcept {
    if (!n)
      return nullptr;
    if (n->left)
      return maximum(n->left);
    Node* p = n->parent;
    while (p && n == p->left) {
      n = p;
      p = p->parent;
    }
    return p;
  }

  void allocate_block() {
    Storage* block = storage_alloc_.allocate(kBlockSize);
    blocks_.push_back(block);
    for (size_type i = 0; i < kBlockSize; ++i) {
      auto* slot = std::construct_at(reinterpret_cast<FreeNode*>(block + i), FreeNode{free_});
      free_ = slot;
    }
  }

  Node* create_node(value_type value) {
    if (!free_)
      allocate_block();
    FreeNode* slot = free_;
    free_ = free_->next;
    return std::construct_at(reinterpret_cast<Node*>(slot), std::move(value));
  }

  void destroy_node(Node* node) noexcept {
    std::destroy_at(node);
    auto* slot = std::construct_at(reinterpret_cast<FreeNode*>(node), FreeNode{free_});
    free_ = slot;
  }

  void release_blocks() noexcept {
    for (auto* block : blocks_)
      storage_alloc_.deallocate(block, kBlockSize);
    blocks_.clear();
    free_ = nullptr;
  }

  void link_node(Node* parent, Node* node,
                 const decltype(key_of_(std::declval<value_type&>()))& k) {
    if (!parent) {
      root_ = node;
      return;
    }
    if (comp_(k, key_of_(parent->value)))
      parent->left = node;
    else
      parent->right = node;
  }

  static Color color_of(Node* n) noexcept {
    return n ? n->color : Color::Black;
  }

  void rotate_left(Node* x) noexcept {
    Node* y = x->right;
    x->right = y->left;
    if (y->left)
      y->left->parent = x;

    y->parent = x->parent;
    if (!x->parent)
      root_ = y;
    else if (x == x->parent->left)
      x->parent->left = y;
    else
      x->parent->right = y;

    y->left = x;
    x->parent = y;
  }

  void rotate_right(Node* x) noexcept {
    Node* y = x->left;
    x->left = y->right;
    if (y->right)
      y->right->parent = x;

    y->parent = x->parent;
    if (!x->parent)
      root_ = y;
    else if (x == x->parent->right)
      x->parent->right = y;
    else
      x->parent->left = y;

    y->right = x;
    x->parent = y;
  }

  void insert_fixup(Node* z) noexcept {
    while (z->parent && z->parent->color == Color::Red) {
      Node* parent = z->parent;
      Node* grand = parent->parent;
      if (parent == grand->left) {
        Node* uncle = grand->right;
        if (color_of(uncle) == Color::Red) {
          parent->color = Color::Black;
          uncle->color = Color::Black;
          grand->color = Color::Red;
          z = grand;
        } else {
          if (z == parent->right) {
            z = parent;
            rotate_left(z);
            parent = z->parent;
            grand = parent->parent;
          }
          parent->color = Color::Black;
          grand->color = Color::Red;
          rotate_right(grand);
        }
      } else {
        Node* uncle = grand->left;
        if (color_of(uncle) == Color::Red) {
          parent->color = Color::Black;
          uncle->color = Color::Black;
          grand->color = Color::Red;
          z = grand;
        } else {
          if (z == parent->left) {
            z = parent;
            rotate_right(z);
            parent = z->parent;
            grand = parent->parent;
          }
          parent->color = Color::Black;
          grand->color = Color::Red;
          rotate_left(grand);
        }
      }
    }
    root_->color = Color::Black;
  }

  void transplant(Node* u, Node* v) noexcept {
    if (!u->parent)
      root_ = v;
    else if (u == u->parent->left)
      u->parent->left = v;
    else
      u->parent->right = v;
    if (v)
      v->parent = u->parent;
  }

  void erase_node(Node* z) noexcept {
    Node* y = z;
    Node* x = nullptr;
    Node* x_parent = nullptr;
    Color y_original = y->color;

    if (!z->left) {
      x = z->right;
      x_parent = z->parent;
      transplant(z, z->right);
    } else if (!z->right) {
      x = z->left;
      x_parent = z->parent;
      transplant(z, z->left);
    } else {
      y = minimum(z->right);
      y_original = y->color;
      x = y->right;
      if (y->parent == z) {
        x_parent = y;
        if (x)
          x->parent = y;
      } else {
        x_parent = y->parent;
        transplant(y, y->right);
        y->right = z->right;
        y->right->parent = y;
      }
      transplant(z, y);
      y->left = z->left;
      y->left->parent = y;
      y->color = z->color;
    }

    destroy_node(z);
    --size_;
    if (y_original == Color::Black)
      erase_fixup(x, x_parent);
  }

  void erase_fixup(Node* x, Node* parent) noexcept {
    while ((x != root_) && color_of(x) == Color::Black) {
      if (x == (parent ? parent->left : nullptr)) {
        Node* w = parent ? parent->right : nullptr;
        if (color_of(w) == Color::Red) {
          w->color = Color::Black;
          parent->color = Color::Red;
          rotate_left(parent);
          w = parent->right;
        }
        if (color_of(w ? w->left : nullptr) == Color::Black &&
            color_of(w ? w->right : nullptr) == Color::Black) {
          if (w)
            w->color = Color::Red;
          x = parent;
          parent = x ? x->parent : nullptr;
        } else {
          if (color_of(w ? w->right : nullptr) == Color::Black) {
            if (w && w->left)
              w->left->color = Color::Black;
            if (w)
              w->color = Color::Red;
            if (w)
              rotate_right(w);
            w = parent ? parent->right : nullptr;
          }
          if (w)
            w->color = parent ? parent->color : Color::Black;
          if (parent)
            parent->color = Color::Black;
          if (w && w->right)
            w->right->color = Color::Black;
          if (parent)
            rotate_left(parent);
          x = root_;
          parent = nullptr;
        }
      } else {
        Node* w = parent ? parent->left : nullptr;
        if (color_of(w) == Color::Red) {
          w->color = Color::Black;
          parent->color = Color::Red;
          rotate_right(parent);
          w = parent->left;
        }
        if (color_of(w ? w->right : nullptr) == Color::Black &&
            color_of(w ? w->left : nullptr) == Color::Black) {
          if (w)
            w->color = Color::Red;
          x = parent;
          parent = x ? x->parent : nullptr;
        } else {
          if (color_of(w ? w->left : nullptr) == Color::Black) {
            if (w && w->right)
              w->right->color = Color::Black;
            if (w)
              w->color = Color::Red;
            if (w)
              rotate_left(w);
            w = parent ? parent->left : nullptr;
          }
          if (w)
            w->color = parent ? parent->color : Color::Black;
          if (parent)
            parent->color = Color::Black;
          if (w && w->left)
            w->left->color = Color::Black;
          if (parent)
            rotate_right(parent);
          x = root_;
          parent = nullptr;
        }
      }
    }
    if (x)
      x->color = Color::Black;
  }

  void destroy_subtree(Node* n) noexcept {
    if (!n)
      return;
    destroy_subtree(n->left);
    destroy_subtree(n->right);
    destroy_node(n);
  }

public:
  class iterator {
  public:
    using value_type = Value;
    using difference_type = std::ptrdiff_t;
    using pointer = Value*;
    using reference = Value&;
    using iterator_category = std::bidirectional_iterator_tag;

    iterator() noexcept : node_(nullptr), tree_(nullptr) {}

    reference operator*() const {
      return node_->value;
    }
    pointer operator->() const {
      return std::addressof(node_->value);
    }

    iterator& operator++() {
      node_ = successor(node_);
      return *this;
    }

    iterator operator++(int) {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    iterator& operator--() {
      if (!node_)
        node_ = maximum(tree_->root_);
      else
        node_ = predecessor(node_);
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
    friend class RbTree;
    friend class const_iterator;
    iterator(Node* node, RbTree* tree) noexcept : node_(node), tree_(tree) {}
    Node* node_;
    RbTree* tree_;
  };

  class const_iterator {
  public:
    using value_type = const Value;
    using difference_type = std::ptrdiff_t;
    using pointer = const Value*;
    using reference = const Value&;
    using iterator_category = std::bidirectional_iterator_tag;

    const_iterator() noexcept : node_(nullptr), tree_(nullptr) {}
    const_iterator(iterator it) noexcept : node_(it.node_), tree_(it.tree_) {}

    reference operator*() const {
      return node_->value;
    }
    pointer operator->() const {
      return std::addressof(node_->value);
    }

    const_iterator& operator++() {
      node_ = successor(const_cast<Node*>(node_));
      return *this;
    }

    const_iterator operator++(int) {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    const_iterator& operator--() {
      if (!node_)
        node_ = maximum(const_cast<Node*>(tree_->root_));
      else
        node_ = predecessor(const_cast<Node*>(node_));
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
    friend class RbTree;
    const_iterator(const Node* node, const RbTree* tree) noexcept : node_(node), tree_(tree) {}
    const Node* node_;
    const RbTree* tree_;
  };
};
