#pragma once

#include "unique-ptr/unique_ptr.hpp"

#include <array>
#include <cstddef>
#include <string_view>

class Trie {
public:
  Trie() : root_(new Node{}) {}

  void insert(std::string_view word) {
    Node* node = root_.get();
    for (unsigned char ch : word) {
      auto& child = node->children[ch];
      if (!child)
        child.reset(new Node{});
      node = child.get();
    }
    node->terminal = true;
  }

  bool contains(std::string_view word) const {
    const Node* node = root_.get();
    for (unsigned char ch : word) {
      const auto& child = node->children[ch];
      if (!child)
        return false;
      node = child.get();
    }
    return node->terminal;
  }

  void erase(std::string_view word) {
    (void)erase_impl(root_.get(), word, 0);
  }

private:
  struct Node {
    bool terminal = false;
    std::array<unique_ptr<Node>, 256> children{};
  };

  unique_ptr<Node> root_;

  static bool has_children(const Node* node) {
    for (const auto& c : node->children) {
      if (c)
        return true;
    }
    return false;
  }

  static bool erase_impl(Node* node, std::string_view word, std::size_t pos) {
    if (!node)
      return false;
    if (pos == word.size()) {
      node->terminal = false;
      return !has_children(node);
    }

    const unsigned char ch = static_cast<unsigned char>(word[pos]);
    auto& child = node->children[ch];
    if (!child)
      return false;

    const bool should_delete_child = erase_impl(child.get(), word, pos + 1);
    if (should_delete_child)
      child.reset();

    return !node->terminal && !has_children(node);
  }
};
