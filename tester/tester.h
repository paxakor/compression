#pragma once

#include <string>
#include <vector>
#include <experimental/string_view>
#include "codec.h"

using StringViewVector = std::vector< std::experimental::string_view >;

class Tester {
public:
  void learn_codec();
  void read_data(const std::string&);
  void set_codec(Codecs::CodecIFace&);
  void test_correctness();
  void test_decode();
  void test_encode();
protected:
  Codecs::CodecIFace* codec;
  StringViewVector data;
  StringViewVector encoded;
  StringViewVector decoded;
};
