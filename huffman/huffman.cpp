#include <iostream>  // for debug
#include "common/defs.h"
#include "common/utils.h"
#include "huffman/bitstream.h"
#include "huffman/huffman.h"

namespace Codecs {

void HuffmanCodec::encode(string& encoded, const string_view& raw) const {
  char code = 0;
  size_t i = 0;
  for (const auto& ch : raw) {
    const auto& code_it = this->table.find(ch);
    if (code_it == this->table.end()) {
      std::cerr << "FUCK THIS SHIT: " << ch << std::endl;  // TODO
      continue;
    }
    for (const auto& bit : code_it->second) {
      if (i == CHAR_SIZE) {
        encoded.push_back(code);
        code = 0;
        i = 0;
      }
      code = (code << 1) + bit;
      ++i;
    }
  }
  encoded.push_back(code << (CHAR_SIZE - i));
  encoded.push_back(static_cast<char>(i));
}

void HuffmanCodec::decode(string& raw, const string_view& encoded) const {
  const size_t rest = encoded.back();
  bitstream bs(encoded, (encoded.size() - 2) * CHAR_SIZE + rest);
  while (!bs.ended()) {
    raw.push_back(this->tree.find_char(bs));
  }
}

string HuffmanCodec::save() const {
  string res;
  res += save_int(this->tree.size());
  for (const auto& nd : this->tree) {
    res += nd.save();
  }
  res += save_int(this->table.size());
  for (const auto& ch : this->table) {
    res += ch.first;
    res += save_bools(ch.second);
  }
  return res;
}

void HuffmanCodec::load(const string_view& config) {
  string config_str = config.to_string();
  string::iterator iter = config_str.begin();
  const string::iterator end = config_str.end();
  const size_t tree_sz = load_int(iter, end);
  this->tree.resize(tree_sz);
  for (size_t i = 0; i < tree_sz; ++i) {
    this->tree.push_back(Node(iter, end));
  }
  const size_t table_sz = load_int(iter, end);
  for (size_t i = 0; i < table_sz; ++i) {
    char ch = *(iter++);
    auto vec = load_bools(iter, end);
    this->table.insert({ch, vec});
  }
}

size_t HuffmanCodec::sample_size(size_t records_total) const {
  return records_total * 1;
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
    Node nd(ch.first);
    size_t position = this->tree.add_node(nd);
    heap.push({ch.second, position});
  }
  return heap;
}

void HuffmanCodec::build_tree(Heap& heap) {
  while (heap.size() > 1) {
    auto a = heap.top();
    heap.pop();
    auto b = heap.top();
    heap.pop();
    heap.push({a.first + b.first, this->tree.size()});
    this->tree.add_node(Node(a.second, b.second));
  }
  this->tree.back().parent = this->tree.size();
}

void HuffmanCodec::build_table() {
  for (size_t i = 0; i < (this->tree.size() + 1) / 2; ++i) {
    const auto ch = this->tree[i].str;
    this->table.insert({ch, this->tree.find_way(ch)});
  }
}

void HuffmanCodec::reset() {
  this->tree.clear();
  this->table.clear();
}

}  // namespace Codecs
