// Copyright 2016, Pavel Korozevtsev.

#include <algorithm>
#include <string>
#include <vector>
#include "trie.h"

namespace Trie {

Node::Node()
  : children(256, 0) {}

Node::Node(size_t p, char ch)
  : children(256, 0)
  , parent(p)
  , prev_char(ch) {}

Trie::Trie() {
  this->nodes.resize(1);
}

const std::vector<Node>& Trie::data() const {
  return this->nodes;
}

void Trie::clear() {
  this->nodes.clear();
  this->nodes.resize(1);
}

void Trie::add_all_chars() {
  char ch = 0;
  do {
    const auto ch_str = std::string(1, ch);
    if (this->count(ch_str) == 0) {
      this->add(ch_str);
    }
  } while (++ch != 0);
}

const std::string Trie::get_string(size_t nd_ptr) const {
  std::string res;
  while (nd_ptr != 0) {
    res.push_back(this->nodes[nd_ptr].prev_char);
    nd_ptr = this->nodes[nd_ptr].parent;
  }
  std::reverse(res.begin(), res.end());
  return res;
}

} // namespace Trie
