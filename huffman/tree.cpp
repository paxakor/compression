#include "huffman/tree.h"
#include <iostream>

Node::Node()
  : is_left_child(false)
  , parent(0)
  , child_l(0)
  , child_r(0)
  , str("")
{}

Node::Node(std::string s)
  : is_left_child(false)
  , parent(0)
  , child_l(0)
  , child_r(0)
  , str(s)
{}

Node::Node(size_t l, size_t r, std::string s)
  : is_left_child(false)
  , parent(0)
  , child_l(l)
  , child_r(r)
  , str(s)
{}

Node::Node(const Node& nd)
  : is_left_child(nd.is_left_child)
  , parent(nd.parent)
  , child_l(nd.child_l)
  , child_r(nd.child_r)
  , str(nd.str)
{}

Node& Node::operator=(const Node& nd) {
  this->is_left_child = nd.is_left_child;
  this->parent = nd.parent;
  this->child_l = nd.child_l;
  this->child_r = nd.child_r;
  this->str = nd.str;
  return *this;
}

size_t Tree::add_node(const Node& nd) {
  size_t position = this->size();
  this->push_back(nd);
  this->at(nd.child_l).is_left_child = true;
  this->at(nd.child_l).parent = position;
  this->at(nd.child_r).is_left_child = false;
  this->at(nd.child_r).parent = position;
  return position;
}

std::vector<bool> Tree::find_way(size_t iter) const {
  std::vector<bool> res;
  while (iter < this->size()) {
    res.push_back(this->at(iter).is_left_child);
    iter = this->at(iter).parent;
  }
  return res;
}
