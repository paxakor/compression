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
  auto mv = log_char_size;
  for (const auto& ch : raw) {
    const auto& code_str = this->table[static_cast<uint8_t>(ch)][mv];
    mv = code_str[0];
    // guaranteed: code_str.size() >= 2
    const auto last = code_str.size() - 1;
    code ^= code_str[1];
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
  encoded[0] ^= static_cast<char>(mv) << (CHAR_SIZE - log_char_size);
}

void HuffmanCodec::decode(string& raw, const string_view& encoded) const {
  const size_t rest = ((encoded[0] >> (CHAR_SIZE - log_char_size)) &
    ((1 << log_char_size) - 1));
  const size_t size = (encoded.size() - 1) * CHAR_SIZE + rest;
  auto index = encoded.begin();
  auto iter = log_char_size;
  uint8_t ch = static_cast<uint8_t>(*index) << iter;
  uint8_t next_ch = static_cast<uint8_t>(*(++index));
  ch ^= (next_ch >> (CHAR_SIZE - iter));
  next_ch <<= iter;
  const size_t last = this->tree.size() - 1;
  for (ssize_t j = size - iter; j > 0;) {
    uint16_t pos = last;
    const Node* const tree_ptr = &this->tree.front();
    while (!tree_ptr[pos].leaf) {
      const auto pair = this->tree_table[ch][pos - (my256 - 1)];
      const size_t wasted = pair.first;
      pos = pair.second;
      j -= wasted;
      ch <<= wasted;
      ch ^= (next_ch >> (CHAR_SIZE - wasted));
      next_ch <<= wasted;
      iter += wasted;
      if (iter >= CHAR_SIZE) {
        iter -= CHAR_SIZE;
        next_ch = *(++index);
        ch ^= (next_ch >> (CHAR_SIZE - iter));
        next_ch = next_ch << iter;
      }
    }
    raw.push_back(tree_ptr[pos].sym);
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
  this->find_all_ways();
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
}

void HuffmanCodec::build_table() {
  this->table = new string* [my256];
  for (size_t i = 0; i < my256; ++i) {
    string*& codes = this->table[static_cast<uint8_t>(this->tree[i].sym)];
    codes = new string[CHAR_SIZE];
    for (size_t j = 0; j < CHAR_SIZE; ++j) {
      codes[j] = bools_to_string(this->tree.get_code(i), j);
    }
  }
}

void HuffmanCodec::find_all_ways(){
  this->tree_table = new std::pair<uint8_t, uint16_t>*[my256];
  uint8_t ch = 0;
  do {
    uint8_t pos = 0;
    this->tree_table[ch] = new std::pair<uint8_t, uint16_t>[my256];
    do {
      this->tree_table[ch][pos] = this->tree.find_way(ch, pos + (my256 - 1));
    } while (++pos != 0);
  } while (++ch != 0);
}

void HuffmanCodec::reset() {
  this->tree.clear();

  for (size_t i = 0; i < my256; ++i) {
    delete[] this->table[i];
    delete[] this->tree_table[i];
  }
  delete[] this->table;
  delete[] this->tree_table;
  this->table = nullptr;
  this->tree_table = nullptr;
}

}  // namespace Codecs
