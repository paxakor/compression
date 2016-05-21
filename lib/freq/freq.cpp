// Copyright 2016, Pavel Korozevtsev.

#include <iostream>  // DEBUG
#include <cinttypes>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <experimental/string_view>
#include <vector>
#include <utility>
#include "common/defs.h"
#include "freq/freq.h"
#include "freq/trie.h"

namespace Codecs {

void FreqCodec::encode(string& encoded, const string_view& raw) const {
  const auto& trie_data = this->trie.data();
  std::vector<uint16_t> res;
  uint16_t nd_ptr = 0;
  for (auto ch = raw.begin(); ch != raw.end();) {
    const auto iter = trie_data[nd_ptr].children.find(*ch);
    // std::cout << "!! " << *ch << " !! ";
    if (iter != trie_data[nd_ptr].children.end() && iter->second < (1 << 16)) {
      nd_ptr = iter->second;
      ++ch;
      // std::cout << 1 << std::endl;
    } else {
      // std::cout << 2 << std::endl;
      res.push_back(nd_ptr);
      nd_ptr = 0;
    }
  }
  if (nd_ptr != 0) {
    res.push_back(nd_ptr);
  }
  const size_t sz = res.size() * sizeof(uint16_t);
  std::experimental::string_view res_sv(reinterpret_cast<char*>(res.data()), sz);
  encoded = res_sv.to_string();
  // memcpy(&(encoded[0]), &(res[0]), sz);
  // std::cout << "enc: " << raw.to_string() << " -> ";
  for (auto r : res) {
    // std::cout << r << " ";
  } //std::cout << std::endl;
}

void FreqCodec::decode(string& raw, const string_view& encoded) const {
  const auto& trie_data = this->trie.data();
  const size_t sz = encoded.size() / sizeof(uint16_t);
  std::vector<uint16_t> res(sz);
  memcpy(&(res[0]), &(encoded[0]), encoded.size());
  // std::cout << "dec: ";
  for (const auto& nd : res) {
    // std::cout << nd << " ";
    uint16_t nd_ptr = nd;
    std::string tmp;
    while (nd_ptr != 0) {
      const auto prnt = trie_data[nd_ptr].parent;
      char ch = 0;
      for (const auto& child : trie_data[prnt].children) {
        if (child.second == nd_ptr) {
          ch = child.first;
          break;
        } 
      }
      tmp.push_back(ch);
      nd_ptr = prnt;
    }
    std::reverse(tmp.begin(), tmp.end());
    raw += tmp;
  }
  // std::cout << "-> " << raw << std::endl;
}

string FreqCodec::save() const {
}

void FreqCodec::load(const string_view& config) {
}

void FreqCodec::learn(const vector<string_view>& all_samples) {
  constexpr size_t max_cnt = (1 << 16) - my256;
  constexpr size_t max_len = 4;
  Trie::Trie tmp_trie;
  for (size_t iter = 0; iter < all_samples.size(); iter += 2) {
    const auto& sv = all_samples[iter];
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
  // vector< std::pair<size_t, size_t> > counts;
  // size_t i = 0;
  // const auto& nodes = tmp_trie.data();
  // for (const auto& nd : nodes) {
  //   counts.push_back({nd.term, i++});
  // }
  // std::sort(counts.begin(), counts.end(),
  //   std::greater< std::pair<size_t, size_t> >());
  // counts.resize(std::max(max_cnt, counts.size()));

  tmp_trie.remove_odd();
  tmp_trie.add_all_chars();
  // print_all(tmp_trie);
  this->trie = tmp_trie;
  const auto& d = trie.data();
  for (size_t i = 0; i < d.size(); ++i) {
    char ch = 0;
    for (const auto& child : d[d[i].parent].children) {
      if (child.second == i) {
        ch = child.first;
        break;
      } 
    }
    if ('a' <= ch && ch <= 'z') {
      auto j = i;
      char chh = 'T';
      while (j) {
        for (const auto& child : d[d[j].parent].children) {
          if (child.second == j) {
            chh = child.first;
            break;
          } 
        }
        // std::cout << " (" << j << ", " << chh << ") <-";
        j = d[j].parent;
      }
      // std::cout << std::endl;
    }
  }
}

size_t FreqCodec::sample_size(size_t records_total) const {
  const size_t min_size = 1;
  const size_t n = ceil(pow(log2(records_total), 2));  // don't know why
  return std::max(min_size, n);
}

void FreqCodec::reset() {
}

}  // namespace Codecs
