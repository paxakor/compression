#include <iostream>  // for debug
#include <unordered_map>
#include <queue>
#include "huffman/huffman.h"

namespace Codecs {

void HuffmanCodec::encode(string& encoded, const string_view& raw) const {
  encoded = raw.to_string();
}
void HuffmanCodec::decode(string& raw, const string_view& encoded) const {
  raw = encoded.to_string();
}

string HuffmanCodec::save() const {
  return string();
}

void HuffmanCodec::load(const string_view& config) {

}

size_t HuffmanCodec::sample_size(size_t records_total) const {
  return 0.1 * records_total;
}

void HuffmanCodec::learn(const vector<string_view>& all_samples) {
  std::unordered_map<char, size_t> frequency;
  for (const auto& record : all_samples) {
    for (const auto& ch : record) {
      auto it = frequency.find(ch);
      if (it == frequency.end()) {
        frequency.insert({ch, 0});
      }
      ++(it->second);
    }
  }
  std::priority_queue< std::pair<size_t, char> > heap;
  for (const auto& ch : frequency) {
    heap.push({ch.second, ch.first});
  }

}

void HuffmanCodec::reset() {

}

}  // namespace Codecs
