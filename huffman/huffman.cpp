#include <iostream>  // for debug
#include <cmath>
#include <algorithm>
#include "common/defs.h"
#include "common/utils.h"
#include "huffman/huffman.h"

namespace Codecs {

const size_t log_char_size = ceil(log2(CHAR_SIZE));

void HuffmanCodec::encode(string& encoded, const string_view& raw) const {
  char code = 0;
  size_t mv = log_char_size;  // mv shows how many bits are already filled
  for (const auto& ch : raw) {
    const auto code_it = this->table.find(ch);
    if (code_it == this->table.end() || ch == this->rare_char) {
      const auto& code_str = this->table.find(this->rare_char)->second[mv];
//-------------
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
//-------------
      code = code | (ch >> mv);
      encoded.push_back(code);
      code = ch << (CHAR_SIZE - mv);
    } else {
      const auto& code_str = code_it->second[mv];
//-------------
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
//-------------
    }
  }
  encoded.push_back(code);
  encoded[0] |= static_cast<char>(mv) << (CHAR_SIZE - log_char_size);
}

void HuffmanCodec::decode(string& raw, const string_view& encoded) const {
  const size_t rest = ((encoded[0] >> (CHAR_SIZE - log_char_size)) &
    ((1 << log_char_size) - 1));
  const size_t size = (encoded.size() - 1) * CHAR_SIZE + rest;
  size_t index = 0;
  size_t iter = log_char_size;
  char ch = encoded[0] << iter;
  for (size_t j = iter; j < size;) {
    const Node* nd = &this->tree.back();
    while (nd->str == 0) {
      if (ch & (1 << (CHAR_SIZE - 1))) {
        nd = &this->tree[nd->child_l];
      } else {
        nd = &this->tree[nd->child_r];
      }
      ++iter;
      ++j;
      ch = ch << 1;
      if (iter == CHAR_SIZE) {
        ch = encoded[++index];
        iter = 0;
      }
    }
    if (nd->str == this->rare_char) {
      char tmp_ch = ch;
      j += CHAR_SIZE;
      ch = encoded[++index];
      tmp_ch = tmp_ch | ((ch >> (CHAR_SIZE - iter)) &
        ((static_cast<char>(1) << iter) - 1));
      ch = ch << iter;
      raw.push_back(tmp_ch);
    } else {
      raw.push_back(nd->str);
    }
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

  // find rare_char
  char ch = CHAR_MIN;
  do {
    if (frequency.count(ch) == 0 && ch != 0) {
      this->rare_char = ch;
      frequency.insert({ch, 0});
      break;
    }
    ++ch;
  } while (ch != CHAR_MAX);

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
    vector<string> codes(CHAR_SIZE);
    for (size_t j = 0; j < CHAR_SIZE; ++j) {
      codes[j] = bools_to_string(this->tree.find_way(i), j);
    }
    this->table.insert({this->tree[i].str, codes});
  }
}

void HuffmanCodec::reset() {
  this->tree.clear();
  this->table.clear();
}

}  // namespace Codecs
