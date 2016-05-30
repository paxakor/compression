// Copyright 2016, Pavel Korozevtsev.

#pragma once

#include <vector>
#include <utility>
#include "lib/common/defs.h"

using SmallPair = std::pair<uint8_t, uint16_t>;

class Node {
public:
  Node();
  explicit Node(CharT);
  Node(size_t, size_t);
  Node(const Node&);

  bool is_leaf;
  bool is_left_child;
  size_t parent;
  size_t child_l;
  size_t child_r;
  CharT sym;
};

class Tree : public std::vector<Node> {
public:
  size_t add_node(const Node&);
  std::vector<bool> get_code(size_t) const;
  SmallPair find_way(CharT, size_t) const;
};
