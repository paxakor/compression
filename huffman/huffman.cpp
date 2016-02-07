#include "huffman.h"

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

void HuffmanCodec::learn(const StringViewVector& all_samples) {

}

void HuffmanCodec::reset() {

}

}  // namespace Codecs
