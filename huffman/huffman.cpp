#include "huffman.h"
#include <unordered_map>
#include <iostream>  // for debug

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
  std::unordered_map<char, size_t> ch_count;
  size_t total_ch = 0;
  for (const auto& record : all_samples) {
    total_ch += record.size();
    for (const auto& ch : record) {
      auto it = ch_count.find(ch);
      if (it != ch_count.end()) {
        ++(it->second);
      } else {
        ch_count.insert({ch, 0});
      }
    }
  }
  std::unordered_map<char, float> probability;
  for (auto it = ch_count.begin(); it != ch_count.end(); ++it) {
    float p = static_cast<float>(it->second) / total_ch;
    probability.insert({it->first, p});
  }
}

void HuffmanCodec::reset() {

}

}  // namespace Codecs
