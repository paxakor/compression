// Copyright 2016, Pavel Korozevtsev.

#include <string>
#include <vector>
#include "lib/trivial/trivial.h"

namespace Codecs {

void TrivialCodec::encode(string& encoded, const string_view& raw) const {
  encoded = raw.to_string();
}

void TrivialCodec::decode(string& raw, const string_view& encoded) const {
  raw = encoded.to_string();
}

string TrivialCodec::save() const { }

void TrivialCodec::load(const string_view& config) { }

void TrivialCodec::learn(const vector<string_view>& all_samples) { }

size_t TrivialCodec::sample_size(size_t records_total) const {
  return 0;
}

void TrivialCodec::reset() { }

}  // namespace Codecs
