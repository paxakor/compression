// Copyright 2016, Pavel Korozevtsev.

#include <cmath>
#include <cstring>
#include <algorithm>
#include <string>
#include <vector>
#include "lib/common/utils.h"
#include "lib/mixed/mixed.h"

namespace Codecs {

const size_t log_char_size = ceil(log2(CHAR_SIZE));

MixedCodec::MixedCodec(const size_t p)
  : power(p)
  , tree()
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

MixedCodec::~MixedCodec() {
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

void MixedCodec::encode(string& encoded, const string_view& raw) const {
  string tmp;
  this->freq_encode(tmp, raw);
  this->huff_encode(encoded, tmp);
}

void MixedCodec::decode(string& raw, const string_view& encoded) const {
  string tmp;
  this->huff_decode(tmp, encoded);
  this->freq_decode(raw, tmp);
}

using FreqIndexType = uint16_t;

void MixedCodec::freq_encode(string& encoded, const string_view& raw) const {
  const auto& trie_data = this->trie.data();
  std::vector<FreqIndexType> res;
  res.reserve(raw.size());
  FreqIndexType nd_ptr = 0;
  for (auto ch = raw.begin(); ch != raw.end();) {
    const auto iter = trie_data[nd_ptr].children[static_cast<uint8_t>(*ch)];
    if (iter != 0) {
      ++ch;
      nd_ptr = iter;
    } else {
      res.push_back(nd_ptr);
      nd_ptr = 0;
    }
  }
  if (nd_ptr != 0) {
    res.push_back(nd_ptr);
  }
  const size_t sz = res.size() * sizeof(FreqIndexType);
  encoded.resize(sz);
  memcpy(const_cast<char*>(encoded.data()), res.data(), sz);
}

void MixedCodec::freq_decode(string& raw, const string_view& encoded) const {
  std::vector<FreqIndexType> res(encoded.size() / sizeof(FreqIndexType));
  memcpy(const_cast<FreqIndexType*>(res.data()), encoded.data(), encoded.size());
  raw.reserve(encoded.size() * 4);
  for (const auto& nd : res) {
    raw += this->strs[nd];
  }
}

void MixedCodec::freq_learn(const vector<string_view>& all_samples) {
  const size_t max_len = 16.0 * ((this->power + 1.0) / 10.0);
  const size_t max_cnt = (1 << (8 + max_len / 2)) - 256;
  Trie::Trie tmp_trie;
  for (size_t idx = 0; idx < all_samples.size(); idx += 2) {
    const auto& sv = all_samples[idx];
    const auto sv_sz = sv.size();
    for (size_t k = 0; k + max_len < sv_sz; ++k) {
      tmp_trie.add(sv.substr(k, max_len));
    }
    if (max_len <= sv_sz) {
      for (size_t k = 0; k < max_len; ++k) {
        tmp_trie.add(sv.substr(sv_sz - max_len + k, max_len - k));
      }
    } else {
      for (size_t k = 0; k < sv_sz; ++k) {
        tmp_trie.add(sv.substr(k, sv_sz - k));
      }
    }
  }
  tmp_trie.add_all_chars();
  const auto& trie_data = tmp_trie.data();
  std::vector<size_t> frequency(trie_data.size(), 0);
  for (size_t idx = 1; idx < all_samples.size(); idx += 2) {
    const auto& sv = all_samples[idx];
    size_t nd_ptr = 0;
    for (auto ch = sv.begin(); ch != sv.end();) {
      const auto iter = trie_data[nd_ptr].children[static_cast<uint8_t>(*ch)];
      if (iter != 0) {
        ++ch;
      } else {
        ++frequency[nd_ptr];
      }
      nd_ptr = iter;
    }
  }
  std::vector< std::pair<size_t, size_t> > best_nodes;
  for (size_t i = 0; i < frequency.size(); ++i) {
    if (frequency[i] > 0) {
      best_nodes.emplace_back(frequency[i], i);
    }
  }
  std::sort(best_nodes.begin(), best_nodes.end(),
    std::greater< decltype(best_nodes)::value_type >());

  for (size_t i = 0; i < best_nodes.size() &&
    this->trie.data().size() < max_cnt; ++i) {
    this->strs_for_build.push_back(tmp_trie.get_string(best_nodes[i].second));
  }
  this->build_trie();
}

void MixedCodec::huff_encode(string& encoded, const string_view& raw) const {
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

void MixedCodec::huff_decode(string& raw, const string_view& encoded) const {
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

string MixedCodec::save() const {
  string data;
  data.resize(fr_sz);
  memcpy(const_cast<char*>(data.data()), this->frequency, fr_sz);

  string res;
  for (const auto& s : this->strs_for_build) {
    res.push_back(static_cast<uint8_t>(s.size()));
    res += s;
  }
  return data + res;
}

void MixedCodec::load(const string_view& config) {
  this->load_frequency(config);
  this->learn_or_load_all();

  size_t i = fr_sz;
  while (i < config.size()) {
    uint8_t sz = config[i++];
    this->strs_for_build.push_back(config.substr(i, sz).to_string());
    i += sz;
  }
  this->build_trie();
}

void MixedCodec::learn(const vector<string_view>& all_samples) {
  this->freq_learn(all_samples);
  vector<string> encoded(all_samples.size());
  auto iter = encoded.begin();
  for (const auto& smpl : all_samples) {
    this->freq_encode(*(iter++), smpl);
  }
  vector<string_view> encoded_refs(encoded.begin(), encoded.end());
  this->precalc_frequency(encoded_refs);
  this->learn_or_load_all();
}

size_t MixedCodec::sample_size(size_t records_total) const {
  constexpr size_t min_size = 16;
  constexpr size_t max_size = 1e5;
  const size_t n = ceil(sqrt(records_total) / log2(records_total));
  return std::min(std::min(std::max(min_size, n), max_size) *
    this->power, records_total);
}

void MixedCodec::reset() {
  this->trie.clear();
  this->strs.clear();
  this->strs_for_build.clear();
  this->tree.clear();
}

void MixedCodec::build_trie() {
  for (const auto& s : this->strs_for_build) {
    this->trie.add(s);
  }
  this->trie.add_all_chars();
  this->strs.resize(this->trie.data().size());
  for (size_t i = 0; i < strs.size(); ++i) {
    this->strs[i] = this->trie.get_string(i);
  }
}

void MixedCodec::precalc_frequency(const vector<string_view>& all_samples) {
  for (const auto& rec : all_samples) {
    for (const auto& ch : rec) {
      this->frequency[static_cast<UCharT>(ch)] += 1;
    }
  }
}

void MixedCodec::load_frequency(const string_view& config) {
  memcpy(this->frequency, config.data(), fr_sz);
}

void MixedCodec::learn_or_load_all() {
  this->tree.reserve(DICT_SIZE * 2);
  Heap heap = this->build_heap();
  this->build_tree(heap);
  this->build_table();
  this->find_all_ways();
}

Heap MixedCodec::build_heap() {
  Heap heap;
  UCharT ch = 0;
  do {
    const Node nd(ch);
    const size_t position = this->tree.add_node(nd);
    heap.emplace(this->frequency[ch], position);
  } while (++ch != 0);
  return heap;
}

void MixedCodec::build_tree(Heap& heap) {
  while (heap.size() > 1) {
    const auto a = heap.top();
    heap.pop();
    const auto b = heap.top();
    heap.pop();
    const size_t position = this->tree.add_node(Node(a.second, b.second));
    heap.emplace(a.first + b.first, position);
  }
  this->tree.back().parent = this->tree.size();
}

void MixedCodec::build_table() {
  for (size_t i = 0; i < DICT_SIZE; ++i) {
    const auto& codes = this->table[static_cast<UCharT>(this->tree[i].sym)];
    for (size_t j = 0; j < CHAR_SIZE; ++j) {
      codes[j] = bools_to_string(this->tree.get_code(i), j);
    }
  }
}

void MixedCodec::find_all_ways() {
  UCharT ch = 0;
  do {
    UCharT pos = 0;
    do {
      this->tree_table[pos][ch] = this->tree.find_way(ch, pos + DICT_SIZE - 1);
    } while (++pos != 0);
  } while (++ch != 0);
}

}  // namespace Codecs
