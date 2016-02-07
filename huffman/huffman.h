#pragma once

#include "codec.h"

namespace Codecs {

class HuffmanCodec : public CodecIFace {
public:
  void encode(string&, const string_view&) const override;
  void decode(string&, const string_view&) const override;

  string save() const override;
  void load(const string_view&) override;

  size_t sample_size(size_t) const override;
  void learn(const StringViewVector&) override;

  void reset() override;
};

}  // namespace Codecs
