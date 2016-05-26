// Copyright 2016, Pavel Korozevtsev.

#include <iostream>
#include <string>
#include "lib/codec/multi_codec.h"
#include "lib/freq/freq.h"
#include "lib/huffman/huffman.h"
#include "lib/trivial/trivial.h"
#include "testers/help.h"
#include "testers/main_tester/tester.h"
#include "testers/stopwatch.h"

int main(int argc, char const *argv[]) {
  std::string file_name("/tmp/data1");
  if (argc > 1) {
    std::string arg(argv[1]);
    if (arg == "--help" || arg == "-h") {
      std::cout << Help::help_message("tester") << std::endl;
      return 0;
    } else {
      file_name = arg;
    }
  }
  Stopwatch sw("Program");
  Codecs::HuffmanCodec huffman;
  Codecs::FreqCodec freq;
  Codecs::TrivialCodec triv;
  Codecs::MultiCodec main_codec(2, &freq, &huffman);
  Tester tester;
  tester.set_codec(main_codec);
  tester.read_data(file_name);
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
  return 0;
}
