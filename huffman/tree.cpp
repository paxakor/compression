#include "huffman/tree.h"

Node::Node() {
  this->is_left_child = true;
  this->parent = 0;
  this->child_l = 0;
  this->child_r = 0;
}

void Tree::resize(size_t sz) {
  this->tree.resize(sz);
  this->iter = sz;
}

void Tree::push(const Node& nd) {
  if (this->iter == 0) {
    return;
  }
  this->tree[--this->iter] = nd;
  if (nd.is_left_child) {
    this->tree[nd.parent].child_l = this->iter;
  } else {
    this->tree[nd.parent].child_r = this->iter;
  }
  this->tree[nd.child_l].parent = this->iter;
  this->tree[nd.child_r].parent = this->iter;
}
