#include <cmath>
#include <cstring>
#include <algorithm>
#include "common/utils.h"
#include "huffman/huffman.h"

namespace Codecs {

const size_t log_char_size = ceil(log2(CHAR_SIZE));

HuffmanCodec::HuffmanCodec()
  : tree()
  , table(new string*[my256])
  , tree_table(new SmallPair*[my256])
  , frequency(new size_t[my256])
{
  for (size_t i = 0; i < my256; ++i) {
    this->table[i] = new string[CHAR_SIZE];
  }

  for (size_t i = 0; i < my256; ++i) {
    this->tree_table[i] = new SmallPair[my256];
  }

  memset(this->frequency, 0, fr_sz);
}

HuffmanCodec::~HuffmanCodec() {
  for (size_t i = 0; i < my256; ++i) {
    delete[] this->table[i];
  }
  delete[] this->table;

  for (size_t i = 0; i < my256; ++i) {
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
    const auto& code_str = this->table[static_cast<uint8_t>(ch)][mv];
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
  const Node* const tree_ptr = &this->tree.front();
  auto index = encoded.begin();
  auto iter = log_char_size;
  uint8_t ch = *index << iter;
  uint8_t next_ch = *(++index);
  ch ^= (next_ch >> (CHAR_SIZE - iter));
  next_ch <<= iter;
  for (size_t j = iter; j < size;) {
    size_t pos = 2 * my256 - 2;  // (tree.size() - 1) -- position of root node
    while (!tree_ptr[pos].is_leaf) {
      const auto pair = this->tree_table[pos - (my256 - 1)][ch];
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
  memcpy(&data[0], this->frequency, fr_sz);
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
  const size_t min_size = 1;
  const size_t n = ceil(pow(log2(records_total), 2));  // don't know why
  return std::max(min_size, n);
}

void HuffmanCodec::reset() {
  this->tree.clear();
}

void HuffmanCodec::precalc_frequency(const vector<string_view>& all_samples) {
  for (const auto& record : all_samples) {
    for (const auto& ch : record) {
      this->frequency[static_cast<uint8_t>(ch)] += 1;
    }
  }
}

void HuffmanCodec::load_frequency(const string_view& config) {
  memcpy(this->frequency, &config[0], fr_sz);
}

void HuffmanCodec::learn_or_load_all() {
  this->tree.reserve(my256 * 2);
  Heap heap = this->build_heap();
  this->build_tree(heap);
  this->build_table();
  this->find_all_ways();
}

Heap HuffmanCodec::build_heap() {
  Heap heap;
  uint8_t ch = 0;
  do {
    Node nd(ch);
    size_t position = this->tree.add_node(nd);
    heap.push({this->frequency[ch], position});
  } while (++ch != 0);
  return heap;
}

void HuffmanCodec::build_tree(Heap& heap) {
  while (heap.size() > 1) {
    auto a = heap.top();
    heap.pop();
    auto b = heap.top();
    heap.pop();
    size_t position = this->tree.add_node(Node(a.second, b.second));
    heap.push({a.first + b.first, position});
  }
  this->tree.back().parent = this->tree.size();
}

void HuffmanCodec::build_table() {
  for (size_t i = 0; i < my256; ++i) {
    string*& codes = this->table[static_cast<uint8_t>(this->tree[i].sym)];
    for (size_t j = 0; j < CHAR_SIZE; ++j) {
      codes[j] = bools_to_string(this->tree.get_code(i), j);
    }
  }
}

void HuffmanCodec::find_all_ways(){
  uint8_t ch = 0;
  do {
    uint8_t pos = 0;
    do {
      this->tree_table[pos][ch] = this->tree.find_way(ch, pos + (my256 - 1));
    } while (++pos != 0);
  } while (++ch != 0);
}

}  // namespace Codecs
