#pragma once

#include <string>
#include <vector>
#include "lib/codec/codec.h"

using std::string;
using std::vector;

class Tester {
public:
  void learn_codec();
  void read_data(const string&);
  void set_codec(Codecs::CodecIFace&);
  void test_correctness() const;
  void test_decode();
  void test_encode();
  void test_size() const;
protected:
  Codecs::CodecIFace* codec;
  vector<string> data;
  vector<string> decoded;
  vector<string> encoded;
};

size_t print_size(const vector<string>&, const string&);
