// Copyright 2016, Pavel Korozevtsev.

#include <string>
#include <vector>
#include "lib/trivial/trivial.h"

namespace Codecs {

void TrivialCodec::encode(string& encoded, const string& raw) const {
  encoded = raw;
}

void TrivialCodec::decode(string& raw, const string& encoded) const {
  raw = encoded;
}

string TrivialCodec::save() const { }

void TrivialCodec::load(const string& config) { }

void TrivialCodec::learn(const vector<string>& all_samples) { }

size_t TrivialCodec::sample_size(size_t records_total) const {
  return 0;
}

void TrivialCodec::reset() { }

}  // namespace Codecs
