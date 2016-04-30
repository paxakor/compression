#pragma once

#include <cinttypes>
#include "codec/codec.h"
#include "rm_fr/trie.h"

namespace Codecs {

class RmFrCodec : public CodecIFace {
public:
  RmFrCodec();
  ~RmFrCodec();

  void encode(string&, const string_view&) const override;
  void decode(string&, const string_view&) const override;

  string save() const override;
  void load(const string_view&) override;

  size_t sample_size(size_t) const override;
  void learn(const vector<string_view>&) override;

  void reset() override;

protected:

};

}  // namespace Codecs
