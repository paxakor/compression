// Copyright 2016, Pavel Korozevtsev.

#include <cinttypes>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <functional>
#include <string>
#include <utility>
#include <vector>
#include "lib/common/defs.h"
#include "lib/freq/freq.h"
#include "lib/freq/trie.h"

namespace Codecs {

FreqCodec::FreqCodec(size_t p)
  : power(p) { }

using IndexType = uint16_t;

void FreqCodec::encode(string& encoded, const string_view& raw) const {
  const auto& trie_data = this->trie.data();
  std::vector<IndexType> res;
  res.reserve(raw.size());
  IndexType nd_ptr = 0;
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
  const size_t sz = res.size() * sizeof(IndexType);
  encoded.resize(sz);
  memcpy(const_cast<char*>(encoded.data()), res.data(), sz);
}

void FreqCodec::decode(string& raw, const string_view& encoded) const {
  std::vector<IndexType> res(encoded.size() / sizeof(IndexType));
  memcpy(const_cast<IndexType*>(res.data()), encoded.data(), encoded.size());
  raw.reserve(encoded.size() * 4);
  for (const auto& nd : res) {
    raw += this->strs[nd];
  }
}

string FreqCodec::save() const {
  return "";
}

void FreqCodec::load(const string_view& config) {
}

void FreqCodec::learn(const vector<string_view>& all_samples) {
  const size_t max_len = 16.0 * ((this->power + 1.0) / 10.0);
  const size_t max_cnt = (1 << (8 + max_len / 2)) - my256;
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
    this->trie.add(tmp_trie.get_string(best_nodes[i].second));
  }
  this->trie.add_all_chars();

  this->strs.resize(this->trie.data().size());
  for (size_t i = 0; i < strs.size(); ++i) {
    this->strs[i] = this->trie.get_string(i);
  }
}

size_t FreqCodec::sample_size(size_t records_total) const {
  constexpr size_t min_size = 16;
  constexpr size_t max_size = 1e5;
  const size_t n = ceil(sqrt(records_total) / log2(records_total));
  return std::min(std::min(std::max(min_size, n),
    max_size) * this->power, records_total);
}

void FreqCodec::reset() {
}

}  // namespace Codecs
