// Copyright 2016, Pavel Korozevtsev.

#pragma once

#include <exception>
#include <sstream>
#include <string>
#include <vector>
#include <experimental/string_view>

namespace Codecs {

using std::string;
using std::vector;
using std::experimental::string_view;

class CodecIFace {
public:
  virtual void encode(string& encoded, const string_view& raw) const = 0;
  virtual void decode(string& raw, const string_view& encoded) const = 0;

  virtual string save() const = 0;
  virtual void load(const string_view&) = 0;

  virtual size_t sample_size(size_t records_total) const = 0;
  virtual void learn(const vector<string_view>& all_samples) = 0;

  virtual void reset() = 0;
};

}  // namespace Codecs
