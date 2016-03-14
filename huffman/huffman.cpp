#include <cmath>
#include <algorithm>
#include "common/defs.h"
#include "common/utils.h"
#include "huffman/huffman.h"

namespace Codecs {

const size_t log_char_size = ceil(log2(CHAR_SIZE));

void HuffmanCodec::encode(string& encoded, const string_view& raw) const {
  char code = 0;
  // mv shows how many bits are already filled
  size_t mv = log_char_size;
  for (const auto& ch : raw) {
    const auto& code_str = this->table[static_cast<unsigned char>(ch)][mv];
    mv = code_str[0];
    // guaranteed: code_str.size() >= 2
    const size_t last = code_str.size() - 1;
    code = code | code_str[1];
    if (last != 1) {
      encoded.push_back(code);
      for (size_t iter = 2; iter < last; ++iter) {
        encoded.push_back(code_str[iter]);
      }
      code = code_str[last];
    }
    if (mv == 0) {
      encoded.push_back(code);
      code = 0;
    }
  }
  encoded.push_back(code);
  encoded[0] |= static_cast<char>(mv) << (CHAR_SIZE - log_char_size);
}

void HuffmanCodec::decode(string& raw, const string_view& encoded) const {
  const size_t rest = ((encoded[0] >> (CHAR_SIZE - log_char_size)) &
    ((1 << log_char_size) - 1));
  const size_t size = (encoded.size() - 1) * CHAR_SIZE + rest;
  auto index = encoded.begin();
  size_t iter = log_char_size;
  char ch = *index << iter;
  for (size_t j = iter; j < size;) {
    const Node* nd = &this->tree.back();
    while (!nd->leaf) {
      if (ch & (1 << (CHAR_SIZE - 1))) {
        nd = nd->child_l_ptr;
      } else {
        nd = nd->child_r_ptr;
      }
      ++iter;
      ++j;
      ch = ch << 1;
      if (iter == CHAR_SIZE) {
        ch = *(++index);
        iter = 0;
      }
    }
    raw.push_back(nd->sym);
  }
}

string HuffmanCodec::save() const {
  string res;
  return res;
}

void HuffmanCodec::load(const string_view& config) {
}

size_t HuffmanCodec::sample_size(size_t records_total) const {
  const size_t min_size = 1;
  const size_t n = ceil(pow(log2(records_total), 2));  // don't know why
  return std::max(min_size, n);
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

  char ch = CHAR_MIN;
  do {
    if (frequency.find(ch) == frequency.end()) {
      frequency.insert({ch, 0});
    }
  } while (ch++ != CHAR_MAX);

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
  for (auto& nd : this->tree) {
    nd.child_l_ptr = &this->tree[nd.child_l];
    nd.child_r_ptr = &this->tree[nd.child_r];
  }
}

void HuffmanCodec::build_table() {
  size_t my256 = static_cast<size_t>(UCHAR_MAX) + 1;
  this->table = new string* [my256];
  for (size_t i = 0; i < (this->tree.size() + 1) / 2; ++i) {
    string* codes = new string[CHAR_SIZE];
    for (size_t j = 0; j < CHAR_SIZE; ++j) {
      codes[j] = bools_to_string(this->tree.find_way(i), j);
    }
    this->table[static_cast<unsigned char>(this->tree[i].sym)] = codes;
  }
}

void HuffmanCodec::reset() {
  this->tree.clear();

  size_t my256 = static_cast<size_t>(UCHAR_MAX) + 1;
  for (size_t i = 0; i < my256; ++i) {
    delete[] this->table[i];
  }
  delete[] this->table;
  this->table = nullptr;
}

}  // namespace Codecs
