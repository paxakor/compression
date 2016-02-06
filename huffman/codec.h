#pragma once

#include "sample.h"

#include <experimental/algorithm>
#include <experimental/string_view>

#include <exception>
#include <string>
#include <sstream>

namespace Codecs {

using std::string;
using std::experimental::string_view;

class CodecException : public std::exception, public std::ostringstream {
public:
  CodecException() = default;
  CodecException(CodecException&& c) noexcept {
    (*this) = std::move(c);
  }

  CodecException& operator=(CodecException&& c) noexcept {
    descr = std::move(c.descr);
    (std::exception&)(*this) = std::move(c);
    return *this;
  }

  explicit CodecException(const std::string& d)
    : descr(d)
  {}

  const char* what() const noexcept override {
    return descr.c_str();
  }

protected:
  std::string descr;
};

class CodecIFace {
public:
  virtual void encode(string& encoded, const string_view& raw) const = 0;
  virtual void decode(string& raw, const string_view& encoded) const = 0;

  virtual string save() const = 0;
  virtual void load(const string_view&) = 0;

  virtual size_t sample_size(size_t records_total) const = 0;
  virtual void learn(const StringViewVector& all_samples) = 0;

  virtual void reset() = 0;
};

template <typename Iter>
void train(CodecIFace& codec, Iter begin, Iter end, size_t pop_size) {
  StringViewVector vec;
  codec.reset();
  select_sample(vec, begin, end, codec.sample_size(pop_size));
  codec.learn(vec);
}

}  // namespace Codecs
