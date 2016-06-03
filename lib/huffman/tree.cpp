// Copyright 2016, Pavel Korozevtsev.

#include <algorithm>
#include <utility>
#include <vector>
#include "lib/common/defs.h"
#include "lib/common/utils.h"
#include "lib/huffman/tree.h"

Node::Node()
  : is_leaf(false)
  , is_left_child(false)
  , parent(0)
  , child_l(0)
  , child_r(0)
  , sym(0)
{}

Node::Node(CharT s)
  : is_leaf(true)
  , is_left_child(false)
  , parent(0)
  , child_l(0)
  , child_r(0)
  , sym(s)
{}

Node::Node(size_t l, size_t r)
  : is_leaf(false)
  , is_left_child(false)
  , parent(0)
  , child_l(l)
  , child_r(r)
  , sym(0)
{}

Node::Node(const Node& nd)
  : is_leaf(nd.is_leaf)
  , is_left_child(nd.is_left_child)
  , parent(nd.parent)
  , child_l(nd.child_l)
  , child_r(nd.child_r)
  , sym(nd.sym)
{}

size_t Tree::add_node(const Node& nd) {
  size_t position = this->size();
  this->push_back(nd);
  this->at(nd.child_l).is_left_child = true;
  this->at(nd.child_l).parent = position;
  this->at(nd.child_r).is_left_child = false;
  this->at(nd.child_r).parent = position;
  return position;
}

std::vector<bool> Tree::get_code(size_t pos) const {
  std::vector<bool> vec;
  while (this->at(pos).parent < this->size()) {
    vec.push_back(this->at(pos).is_left_child);
    pos = this->at(pos).parent;
  }
  std::reverse(vec.begin(), vec.end());
  return vec;
}

SmallPair Tree::find_way(uint8_t ch, size_t pos) const {
  size_t i = CHAR_SIZE;
  while (i != 0 && !this->at(pos).is_leaf) {
    if (ch & (1 << (CHAR_SIZE - 1))) {
      pos = this->at(pos).child_l;
    } else {
      pos = this->at(pos).child_r;
    }
    ch <<= 1;
    --i;
  }
  return {CHAR_SIZE - i, pos};
}
