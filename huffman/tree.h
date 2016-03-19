#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <unordered_map>

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
  char sym;
  bool leaf;
};

class Tree : public std::vector<Node> {
public:
  size_t add_node(const Node&);
  std::vector<bool> get_code(size_t) const;
  std::pair<uint8_t, uint16_t> find_way(char, size_t) const;
};
