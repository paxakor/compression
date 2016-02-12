#include <iostream>  // for debug
#include "huffman/huffman.h"

namespace Codecs {

void HuffmanCodec::encode(string& encoded, const string_view& raw) const {
  encoded = raw.to_string();
}
void HuffmanCodec::decode(string& raw, const string_view& encoded) const {
  raw = encoded.to_string();
}

string HuffmanCodec::save() const {
  string res;
  
  return res;
}

void HuffmanCodec::load(const string_view& config) {

}

size_t HuffmanCodec::sample_size(size_t records_total) const {
  return 0.1 * records_total;
}

void HuffmanCodec::learn(const vector<string_view>& all_samples) {
  Heap heap = this->precalc(all_samples);
  this->build_tree(heap);
  this->build_table();
}

Heap HuffmanCodec::precalc(const vector<string_view>& all_samples) {
  std::unordered_map<char, size_t> frequency;
  for (const auto& record : all_samples) {
    for (const auto& ch : record) {
      auto it = frequency.find(ch);
      if (it == frequency.end()) {
        frequency.insert({ch, 1});
      } else {
        ++(it->second);
      }
    }
  }
  Heap heap;
  for (const auto& ch : frequency) {
    Node nd(string(1, ch.first));
    size_t position = this->tree.add_node(nd);
    heap.push({ch.second, position});
    this->table.insert({ch.first, {}});
  }
  return heap;
}

void HuffmanCodec::build_tree(Heap& heap) {
  size_t iter = this->tree.size();
  while (heap.size() > 1) {
    auto a = heap.top();
    heap.pop();
    auto b = heap.top();
    heap.pop();
    heap.push({a.first + b.first, iter++});
    Node na = this->tree[a.second];
    Node nb = this->tree[b.second];
    Node result(a.second, b.second, na.str + nb.str);
    this->tree.add_node(result);
  }
  this->tree.back().parent = this->tree.size();
}

void HuffmanCodec::build_table() {
  size_t abc = this->table.size();
  size_t tsz = 2 * abc - 1;
  for (size_t i = 0; i < abc; ++i) {
    auto ch = this->tree[tsz - i - 1].str[0];
    auto it = this->table.find(ch);
    it->second = this->tree.find_way(i);
  }
}

void HuffmanCodec::reset() {
  this->tree.clear();
  this->table.clear();
}

}  // namespace Codecs
