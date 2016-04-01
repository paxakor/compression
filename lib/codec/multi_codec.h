#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include "codec/codec.h"

namespace Codecs {

using std::string;
using std::vector;

class MultiCodec : public CodecIFace {
public:
  MultiCodec(size_t, CodecIFace*, ...);

  void push(CodecIFace*);
  void pop();

  void encode(string&, const string_view&) const override;
  void decode(string&, const string_view&) const override;

  string save() const override;
  void load(const string_view&) override;

  size_t sample_size(size_t) const override;
  void learn(const vector<string_view>&) override;

  void reset() override;

protected:
  template <typename Function>
  void for_each_codec(Function);

  template <typename Function>
  void for_each_codec(Function) const;

  std::vector<CodecIFace*> codecs_list;
};

template <typename Function>
void MultiCodec::for_each_codec(Function f) {
  std::for_each(this->codecs_list.begin(), this->codecs_list.end(), f);
}

template <typename Function>
void MultiCodec::for_each_codec(Function f) const {
  std::for_each(this->codecs_list.cbegin(), this->codecs_list.cend(), f);
}

}  // namespace Codecs
