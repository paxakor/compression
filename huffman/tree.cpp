#include <algorithm>
#include "common/defs.h"
#include "common/utils.h"
#include "huffman/tree.h"

Node::Node()
  : is_left_child(false)
  , parent(0)
  , child_l(0)
  , child_r(0)
  , child_l_ptr(nullptr)
  , child_r_ptr(nullptr)
  , sym(0)
  , leaf(false)
{}

Node::Node(char s)
  : is_left_child(false)
  , parent(0)
  , child_l(0)
  , child_r(0)
  , child_l_ptr(nullptr)
  , child_r_ptr(nullptr)
  , sym(s)
  , leaf(true)
{}

Node::Node(size_t l, size_t r)
  : is_left_child(false)
  , parent(0)
  , child_l(l)
  , child_r(r)
  , child_l_ptr(nullptr)
  , child_r_ptr(nullptr)
  , sym(0)
  , leaf(false)
{}

Node::Node(const Node& nd)
  : is_left_child(nd.is_left_child)
  , parent(nd.parent)
  , child_l(nd.child_l)
  , child_r(nd.child_r)
  , child_l_ptr(nd.child_l_ptr)
  , child_r_ptr(nd.child_r_ptr)
  , sym(nd.sym)
  , leaf(nd.leaf)
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

size_t Tree::find_way(char ch, size_t pos) const {
  size_t i = CHAR_SIZE;
  while (i != 0 && !this->at(pos).leaf) {
    if (ch & (1 << (CHAR_SIZE - 1))) {
      pos = this->at(pos).child_l;
    } else {
      pos = this->at(pos).child_r;
    }
    ch = ch << 1;
    --i;
  }
  return ((CHAR_SIZE - i) << (2 * CHAR_SIZE)) | pos;
}
