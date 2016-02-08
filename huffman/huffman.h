#pragma once

#include "codec/codec.h"
#include "huffman/tree.h"

namespace Codecs {

class HuffmanCodec : public CodecIFace {
public:
  void encode(string&, const string_view&) const override;
  void decode(string&, const string_view&) const override;

  string save() const override;
  void load(const string_view&) override;

  size_t sample_size(size_t) const override;
  void learn(const vector<string_view>&) override;

  void reset() override;
protected:
  Tree tree;
};

}  // namespace Codecs
