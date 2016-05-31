// Copyright 2016, Pavel Korozevtsev.

#pragma once

#include <string>
#include <vector>
#include "lib/codec/codec.h"

namespace Codecs {

class TrivialCodec : public CodecIFace {
public:
  void encode(string&, const string&) const override;
  void decode(string&, const string&) const override;

  string save() const override;
  void load(const string&) override;

  size_t sample_size(size_t) const override;
  void learn(const vector<string>&) override;

  void reset() override;
};

}  // namespace Codecs
