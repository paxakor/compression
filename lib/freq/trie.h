#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include <unordered_map>

class Node {
public:
  Node();
  Node(size_t);
  Node(const Node&);
  size_t parent;
  std::unordered_map<char, size_t> children;
  bool term;
};

class Trie {
public:
  Trie();
  void add(const std::string&);
  bool find(const std::string&) const;
  std::vector<Node> nodes;
};
