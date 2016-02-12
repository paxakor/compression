#pragma once

#include <stddef.h>
#include <string>
#include <vector>

class Node {
public:
  Node();
  Node(std::string);
  Node(size_t, size_t, std::string);
  Node(const Node&);
  Node(std::string::iterator&, const std::string::iterator&);
  Node& operator=(const Node&);
  std::string save() const;

  bool is_left_child;
  size_t parent;
  size_t child_l;
  size_t child_r;
  std::string str;
};

class Tree : public std::vector<Node> {
public:
  size_t add_node(const Node&);
  std::vector<bool> find_way(size_t) const;
};
