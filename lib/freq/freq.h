// Copyright 2016, Pavel Korozevtsev.

#pragma once

#include <cinttypes>
#include <string>
#include <vector>
#include "lib/codec/codec.h"
#include "lib/freq/trie.h"

namespace Codecs {

using std::string;
using std::vector;

class FreqCodec : public CodecIFace {
public:
  FreqCodec(size_t);

  void encode(string&, const string&) const override;
  void decode(string&, const string&) const override;

  string save() const override;
  void load(const string&) override;

  size_t sample_size(size_t) const override;
  void learn(const vector<string>&) override;

  void reset() override;

protected:
  void build_trie();
  Trie::Trie trie;
  vector<string> strs;
  vector<string> strs_for_build;
  const size_t power;
};

}  // namespace Codecs
