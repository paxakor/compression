#pragma once

#include <experimental/string_view>
#include <string>
#include <vector>
#include "codec.h"

class Tester {
public:
  using StringViewVector = std::vector< std::experimental::string_view >;
  void learn_codec();
  void read_data(const std::string&);
  void set_codec(Codecs::CodecIFace&);
  void test_correctness();
  void test_decode();
  void test_encode();
protected:
  Codecs::CodecIFace* codec;
  std::vector< std::string > data_str;
  StringViewVector data;
  StringViewVector encoded;
  StringViewVector decoded;
};
