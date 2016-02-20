#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <unordered_map>
#include "huffman/bitstream.h"

class Node {
public:
  Node();
  Node(char);
  Node(size_t, size_t);
  Node(const Node&);
  Node(std::string::iterator&, const std::string::iterator&);
  std::string save() const;

  bool is_left_child;
  size_t parent;
  size_t child_l;
  size_t child_r;
  char str;
};

class Tree : public std::vector<Node> {
public:
  size_t add_node(const Node&);
  std::vector<bool> find_way(char) const;
  char find_char(bitstream&) const;
protected:
  std::unordered_map<char, size_t> char_to_index;
};
