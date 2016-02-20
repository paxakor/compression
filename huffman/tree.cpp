#include <algorithm>
#include "common/utils.h"
#include "huffman/tree.h"

Node::Node()
  : is_left_child(false)
  , parent(0)
  , child_l(0)
  , child_r(0)
  , str(0)
{}

Node::Node(char s)
  : is_left_child(false)
  , parent(0)
  , child_l(0)
  , child_r(0)
  , str(s)
{}

Node::Node(size_t l, size_t r)
  : is_left_child(false)
  , parent(0)
  , child_l(l)
  , child_r(r)
  , str(0)
{}

Node::Node(const Node& nd)
  : is_left_child(nd.is_left_child)
  , parent(nd.parent)
  , child_l(nd.child_l)
  , child_r(nd.child_r)
  , str(nd.str)
{}

Node::Node(std::string::iterator& iter, const std::string::iterator& end) {
  this->is_left_child = load_int(iter, end);
  this->parent = load_int(iter, end);
  this->child_l = load_int(iter, end);
  this->child_r = load_int(iter, end);
}

std::string Node::save() const {
  std::string res;
  res += save_int(this->is_left_child);
  res += save_int(this->parent);
  res += save_int(this->child_l);
  res += save_int(this->child_r);
  return res;
}

size_t Tree::add_node(const Node& nd) {
  size_t position = this->size();
  this->push_back(nd);
  this->at(nd.child_l).is_left_child = true;
  this->at(nd.child_l).parent = position;
  this->at(nd.child_r).is_left_child = false;
  this->at(nd.child_r).parent = position;
  this->char_to_index.insert({nd.str, position});
  return position;
}

std::vector<bool> Tree::find_way(char ch) const {
  const auto iter = this->char_to_index.find(ch);
  size_t pos = 0;
  if (iter != this->char_to_index.end()) {
    pos = iter->second;
  } else {
    return {};  // TODO
  }
  std::vector<bool> vec;
  while (this->at(pos).parent < this->size()) {
    vec.push_back(this->at(pos).is_left_child);
    pos = this->at(pos).parent;
  }
  std::reverse(vec.begin(), vec.end());
  return vec;
}

char Tree::find_char(bitstream& bs) const {
  Node nd = this->back();
  while (nd.str == 0) {
    if (bs.get_next()) {
      nd = this->at(nd.child_l);
    } else {
      nd = this->at(nd.child_r);
    }
  }
  return nd.str;
}
