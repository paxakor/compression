// Copyright 2016, Pavel Korozevtsev.

#pragma once

#include <string>
#include <vector>

namespace Codecs {

using std::string;
using std::vector;

class CodecIFace {
public:
  virtual void encode(string& encoded, const string& raw) const = 0;
  virtual void decode(string& raw, const string& encoded) const = 0;

  virtual string save() const = 0;
  virtual void load(const string&) = 0;

  virtual size_t sample_size(size_t records_total) const = 0;
  virtual void learn(const vector<string>& all_samples) = 0;

  virtual void reset() = 0;
};

}  // namespace Codecs
