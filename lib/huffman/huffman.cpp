// Copyright 2016, Pavel Korozevtsev.

#include <cmath>
#include <cstring>
#include <algorithm>
#include <string>
#include <vector>
#include "lib/common/utils.h"
#include "lib/huffman/huffman.h"

namespace Codecs {

const size_t log_char_size = ceil(log2(CHAR_SIZE));

HuffmanCodec::HuffmanCodec()
  : tree()
  , table(new string*[DICT_SIZE])
  , tree_table(new SmallPair*[DICT_SIZE])
  , frequency(new size_t[DICT_SIZE]) {
  for (size_t i = 0; i < DICT_SIZE; ++i) {
    this->table[i] = new string[CHAR_SIZE];
  }

  for (size_t i = 0; i < DICT_SIZE; ++i) {
    this->tree_table[i] = new SmallPair[DICT_SIZE];
  }

  memset(this->frequency, 0, fr_sz);
}

HuffmanCodec::~HuffmanCodec() {
  for (size_t i = 0; i < DICT_SIZE; ++i) {
    delete[] this->table[i];
  }
  delete[] this->table;

  for (size_t i = 0; i < DICT_SIZE; ++i) {
    delete[] this->tree_table[i];
  }
  delete[] this->tree_table;

  delete[] this->frequency;
}

void HuffmanCodec::encode(string& encoded, const string_view& raw) const {
  encoded.reserve(raw.size() * 2);
  char code = 0;
  // mv shows how many bits are already filled
  auto mv = log_char_size;
  for (const auto& ch : raw) {
    const auto& code_str = this->table[static_cast<UCharT>(ch)][mv];
    mv = code_str[0];
    // guaranteed: code_str.size() >= 2
    const auto last = code_str.size() - 1;
    code ^= code_str[1];
    if (__builtin_expect(last != 1, false)) {
      encoded.push_back(code);
      for (size_t iter = 2; iter < last; ++iter) {
        encoded.push_back(code_str[iter]);
      }
      code = code_str[last];
    }
    if (__builtin_expect(mv == 0, false)) {
      encoded.push_back(code);
      code = 0;
    }
  }
  encoded.push_back(code);
  encoded[0] ^= static_cast<char>(mv) << (CHAR_SIZE - log_char_size);
}

void HuffmanCodec::decode(string& raw, const string_view& encoded) const {
  raw.reserve(encoded.size() * 2);
  const size_t rest = ((encoded[0] >> (CHAR_SIZE - log_char_size)) &
    ((1 << log_char_size) - 1));
  const size_t size = (encoded.size() - 1) * CHAR_SIZE + rest;
  const auto tree_ptr = this->tree.data();
  auto index = encoded.begin();
  auto iter = log_char_size;
  UCharT ch = *index << iter;
  UCharT next_ch = *(++index);
  ch ^= (next_ch >> (CHAR_SIZE - iter));
  next_ch <<= iter;
  for (size_t j = iter; j < size;) {
    size_t pos = 2 * DICT_SIZE - 2;  // position of root node
    while (!tree_ptr[pos].is_leaf) {
      const auto pair = this->tree_table[pos - (DICT_SIZE - 1)][ch];
      const size_t wasted = pair.first;
      pos = pair.second;
      j += wasted;
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
  string data;
  data.resize(fr_sz);
  memcpy(const_cast<char*>(data.data()), this->frequency, fr_sz);
  return data;
}

void HuffmanCodec::load(const string_view& config) {
  this->load_frequency(config);
  this->learn_or_load_all();
}

void HuffmanCodec::learn(const vector<string_view>& all_samples) {
  this->precalc_frequency(all_samples);
  this->learn_or_load_all();
}

size_t HuffmanCodec::sample_size(size_t records_total) const {
  constexpr size_t min_size = 16;
  constexpr size_t max_size = 1e5;
  const size_t n = ceil(log2(records_total));  // don't know why
  return std::min(std::min(std::max(min_size, n), max_size), records_total);
}

void HuffmanCodec::reset() {
  this->tree.clear();
}

void HuffmanCodec::precalc_frequency(const vector<string_view>& all_samples) {
  for (const auto& record : all_samples) {
    Wstring_view wrec(record.data(), record.size());
    for (const auto& ch : wrec) {
      this->frequency[static_cast<UCharT>(ch)] += 1;
    }
  }
}

void HuffmanCodec::load_frequency(const string_view& config) {
  memcpy(this->frequency, config.data(), fr_sz);
}

void HuffmanCodec::learn_or_load_all() {
  this->tree.reserve(DICT_SIZE * 2);
  Heap heap = this->build_heap();
  this->build_tree(heap);
  this->build_table();
  this->find_all_ways();
}

Heap HuffmanCodec::build_heap() {
  Heap heap;
  UCharT ch = 0;
  do {
    Node nd(ch);
    size_t position = this->tree.add_node(nd);
    heap.push({this->frequency[ch], position});
  } while (++ch != 0);
  return heap;
}

void HuffmanCodec::build_tree(Heap& heap) {
  while (heap.size() > 1) {
    const auto a = heap.top();
    heap.pop();
    const auto b = heap.top();
    heap.pop();
    size_t position = this->tree.add_node(Node(a.second, b.second));
    heap.push({a.first + b.first, position});
  }
  this->tree.back().parent = this->tree.size();
}

void HuffmanCodec::build_table() {
  for (size_t i = 0; i < DICT_SIZE; ++i) {
    const auto& codes = this->table[static_cast<UCharT>(this->tree[i].sym)];
    for (size_t j = 0; j < CHAR_SIZE; ++j) {
      codes[j] = bools_to_string(this->tree.get_code(i), j);
    }
  }
}

void HuffmanCodec::find_all_ways() {
  UCharT ch = 0;
  do {
    UCharT pos = 0;
    do {
      this->tree_table[pos][ch] = this->tree.find_way(ch, pos + DICT_SIZE - 1);
    } while (++pos != 0);
  } while (++ch != 0);
}

}  // namespace Codecs
