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

void FreqCodec::encode(string& encoded, const string& raw) const {
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
  auto enc_ptr = const_cast<char*>(encoded.data());
  if (enc_ptr != nullptr && res.data() != nullptr) {
    memcpy(enc_ptr, res.data(), sz);
  }
}

void FreqCodec::decode(string& raw, const string& encoded) const {
  std::vector<IndexType> res(encoded.size() / sizeof(IndexType));
  auto res_ptr = const_cast<IndexType*>(res.data());
  if (res_ptr != nullptr && encoded.data() != nullptr) {
    memcpy(res_ptr, encoded.data(), encoded.size());
  }
  raw.reserve(encoded.size() * 4);
  for (const auto& nd : res) {
    raw += this->strs[nd];
  }
}

string FreqCodec::save() const {
  string res;
  for (const auto& s : this->strs_for_build) {
    res.push_back(static_cast<uint8_t>(s.size()));
    res += s;
  }
  return res;
}

void FreqCodec::load(const string& config) {
  for (size_t i = 0; i < config.size();) {
    uint8_t sz = config[i++];
    this->strs_for_build.push_back(config.substr(i, sz));
    i += sz;
  }
  this->build_trie();
}

void FreqCodec::learn(const vector<string>& all_samples) {
  const size_t len = 4 + 12.0 * ((this->power + 1.0) / 10.0);
  const size_t max_cnt = (1 << (8 + len / 2)) - 256;
  Trie::Trie tmp_trie;
  for (size_t idx = 0; idx < all_samples.size(); idx += 2) {
    const auto& sv = all_samples[idx];
    const auto sv_sz = sv.size();
    if (len <= sv_sz) {
      for (size_t k = 0; k + len < sv_sz; k += 2) {
        bool good = true;
        for (size_t l = len; good && k + l < sv_sz && l < 256; l += len) {
          good = tmp_trie.add(sv.substr(k, l));
        }
      }
      for (size_t k = 0; k < len; k += 2) {
        tmp_trie.add(sv.substr(sv_sz - len + k, len - k));
      }
    } else {
      for (size_t k = 0; k < sv_sz; k += 2) {
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
      const auto str = tmp_trie.get_string(i);
      const size_t freq1 = frequency[i];
      const size_t freq2 = freq1 * sqrt(log2(str.size()));
      best_nodes.emplace_back(std::max(freq1, freq2), i);
    }
  }
  std::sort(best_nodes.begin(), best_nodes.end(),
    std::greater< decltype(best_nodes)::value_type >());

  for (size_t i = 0; i < best_nodes.size() &&
    this->strs_for_build.size() < max_cnt; ++i) {
    const auto str = tmp_trie.get_string(best_nodes[i].second);
    this->strs_for_build.push_back(str);
  }
  this->build_trie();
}

void FreqCodec::build_trie() {
  for (const auto& s : this->strs_for_build) {
    this->trie.add(s);
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
  const size_t n = ceil(2 * log2(records_total));
  return std::min(std::min(std::max(min_size, n),
    max_size) * this->power, records_total);
}

void FreqCodec::reset() {
  this->trie.clear();
  this->strs.clear();
  this->strs_for_build.clear();
}

}  // namespace Codecs
