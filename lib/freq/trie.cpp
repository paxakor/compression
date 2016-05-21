// Copyright 2016, Pavel Korozevtsev.

#include <algorithm>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include "trie.h"

namespace Trie {

Node::Node(size_t p)
  : parent(p) {}

Trie::Trie() {
  this->nodes.resize(1);
}

void Trie::remove_odd() {
  for (auto& nd : this->nodes) {
    if (nd.children.size() == 1 || nd.term < 3) {
      nd.term = 0;
    }
  }
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

using int_str = std::pair<size_t, std::string>;

std::vector<int_str> get_all_strings(const Trie& trie) {
  std::vector<int_str> res;
  const auto nodes = trie.data();
  std::stack<int_str> nd_st;
  nd_st.push({0, ""});
  while (!nd_st.empty()) {
    const auto nd = nd_st.top();
    nd_st.pop();
    if (nodes[nd.first].term > 0) {
      res.push_back({nodes[nd.first].term, nd.second});
    }
    for (const auto& child : nodes[nd.first].children) {
      nd_st.push({child.second, nd.second + child.first});
    }
  }
  std::sort(res.begin(), res.end(),
    std::greater<int_str>());
  return res;
}

void print_all(const Trie& trie) {
  const auto res = get_all_strings(trie);
  for (const auto& pp : res) {
    std::cout << pp.first << ": \t\t" << pp.second << std::endl;
  }
}

} // namespace Trie
