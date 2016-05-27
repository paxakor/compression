// Copyright 2016, Pavel Korozevtsev.

#pragma once

#include <cinttypes>
#include <string>
#include <vector>
#include <experimental/string_view>
#include "lib/codec/codec.h"
#include "lib/freq/trie.h"

namespace Codecs {

using std::string;
using std::vector;
using std::experimental::string_view;

class FreqCodec : public CodecIFace {
public:
  FreqCodec(size_t);

  void encode(string&, const string_view&) const override;
  void decode(string&, const string_view&) const override;

  string save() const override;
  void load(const string_view&) override;

  size_t sample_size(size_t) const override;
  void learn(const vector<string_view>&) override;

  void reset() override;

protected:
  Trie::Trie trie;
  std::vector<std::string> strs;
  const size_t power;
};

}  // namespace Codecs
