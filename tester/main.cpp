// Copyright 2016, Pavel Korozevtsev.

#include <ctime>
#include <iostream>
#include "codec/multi_codec.h"
#include "common/utils.h"
#include "freq/freq.h"
#include "huffman/huffman.h"
#include "tester/config.h"
#include "tester/tester.h"

int main() {
  time_t start = time(nullptr);
  Config conf;
  conf.read();

  Codecs::HuffmanCodec huffman;
  Codecs::FreqCodec freq;
  Codecs::MultiCodec main_codec(2, &freq, &huffman);
  Tester tester;
  tester.set_codec(main_codec);
  tester.read_data(conf["data_file"]);
  tester.learn_codec();
  tester.test_encode();

#if 0
  const auto state = main_codec.save();
  main_codec.reset();
  main_codec.load(state);
#endif
#if 1
  tester.test_decode();
  tester.test_size();
  tester.test_correctness();
#endif

  std::cout << "Program ended in " << time(nullptr) - start <<
    " seconds" << std::endl;
  return 0;
}
