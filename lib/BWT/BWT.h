#pragma once

#include <string>
#include <vector>
#include "codec/codec.h"

namespace Codecs {

using std::string;
using std::vector;

class BWTCodec : public CodecIFace {
public:
  void encode(string&, const string_view&) const override;
  void decode(string&, const string_view&) const override;

  string save() const override;
  void load(const string_view&) override;

  size_t sample_size(size_t) const override;
  void learn(const vector<string_view>&) override;

  void reset() override;
};

}  // namespace Codecs
