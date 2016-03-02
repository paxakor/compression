#include <cmath>
#include <algorithm>
#include "common/defs.h"
#include "common/utils.h"
#include "huffman/huffman.h"

namespace Codecs {

const size_t log_char_size = ceil(log2(CHAR_SIZE));

void HuffmanCodec::encode(string& encoded, const string_view& raw) const {
  char code = 0;
  size_t iter = log_char_size;
  for (const auto& ch : raw) {
    const auto code_it = this->table.find(ch);
    if (code_it == this->table.end() || ch == this->rare_char) {
      const auto& char_code = this->table.find(this->rare_char)->second;
      for (const auto& bit : char_code) {
        code = (code << 1) + bit;
        ++iter;
        if (iter == CHAR_SIZE) {
          encoded.push_back(code);
          code = 0;
          iter = 0;
        }
      }
      code = (code << (CHAR_SIZE - iter)) | (ch >> iter);
      encoded.push_back(code);
      code = ch;
    } else {
      for (const auto& bit : code_it->second) {
        code = (code << 1) + bit;
        ++iter;
        if (iter == CHAR_SIZE) {
          encoded.push_back(code);
          code = 0;
          iter = 0;
        }
      }
    }
  }
  encoded.push_back(code << (CHAR_SIZE - iter));
  encoded[0] |= static_cast<char>(iter) << (CHAR_SIZE - log_char_size);
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
  const size_t min_size = 1;
  const double k = 0.05;
  return std::max(min_size, static_cast<size_t>(records_total * k));
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
    this->table.insert({this->tree[i].str, this->tree.find_way(i)});
  }
}

void HuffmanCodec::reset() {
  this->tree.clear();
  this->table.clear();
}

}  // namespace Codecs
