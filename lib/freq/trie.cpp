#include "trie.hpp"

Node::Node()
  : term(false)
{}

Node::Node(size_t p)
  : parent(p)
  , term(false)
{}

Node::Node(const Node& nd) 
  : parent(nd.parent)
  , children(nd.children)
  , term(nd.term)
{}

Trie::Trie() {
  this->nodes.resize(1);
}

void Trie::add(const std::string& str) {
  size_t node_iter = 0;
  for (const auto& ch : str) {
    auto it = this->nodes[node_iter].children.find(ch);
    if (it == this->nodes[node_iter].children.end()) {
      const size_t sz = this->nodes.size();
      this->nodes.push_back(Node(node_iter));
      this->nodes[node_iter].children.insert({ch, sz});
      node_iter = sz;
    } else {
      node_iter = it->second;
    }
  }
  this->nodes[node_iter].term = true;
}

bool Trie::find(const std::string& str) const {
  size_t node_iter = 0;
  for (const auto& ch : str) {
    auto it = this->nodes[node_iter].children.find(ch);
    if (it == this->nodes[node_iter].children.end()) {
      return false;
    } else {
      node_iter = it->second;
    }
  }
  return this->nodes[node_iter].term == true;
}
