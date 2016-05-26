#pragma once

#include <string>
#include "lib/codec/codec.h"

class StreamTester {
public:
  void set_codec(Codecs::CodecIFace&);
  void set_data_file(const std::string&);
  void learn_codec();
  void test_all() const;
protected:
  Codecs::CodecIFace* codec;
  std::string data_file;
};
