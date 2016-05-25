#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include <unordered_map>

namespace Trie {

class Node {
public:
  Node() = default;
  Node(size_t, char);
  Node(const Node&) = default;
  std::unordered_map<char, size_t> children;
  size_t parent = 0;
  size_t term = 0;
  char prev_char = 0;
};

class Trie {
public:
  Trie();
  template <typename Str> void add(const Str&);
  template <typename Str> size_t count(const Str&) const;
  const std::vector<Node>& data() const;
  void clear();
  void add_all_chars();
  const std::string get_string(size_t) const;
protected:
  std::vector<Node> nodes;
};

template <typename Str> 
void Trie::add(const Str& str) {
  size_t node_iter = 0;
  for (const auto& ch : str) {
    const auto it = this->nodes[node_iter].children.find(ch);
    if (it == this->nodes[node_iter].children.end()) {
      const size_t sz = this->nodes.size();
      this->nodes.emplace_back(node_iter, ch);
      this->nodes[node_iter].children.insert({ch, sz});
      node_iter = sz;
    } else {
      node_iter = it->second;
    }
    ++(this->nodes[node_iter].term);
  }
}

template <typename Str> 
size_t Trie::count(const Str& str) const {
  size_t node_iter = 0;
  for (const auto& ch : str) {
    const auto it = this->nodes[node_iter].children.find(ch);
    if (it == this->nodes[node_iter].children.end()) {
      return 0;
    } else {
      node_iter = it->second;
    }
  }
  return this->nodes[node_iter].term;
}

} // namespace Trie
