#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <vector>

class Node {
public:
  Node();
  bool is_left_child;
  uint16_t parent;
  uint16_t child_l;
  uint16_t child_r;
};

class Tree {
public:
  void resize(size_t);
  void push(const Node&);
protected:
  std::vector<Node> tree;
  size_t iter;
};
