// Copyright 2016, Pavel Korozevtsev.

#include <ctime>
#include <iostream>
#include "codec/multi_codec.h"
#include "freq/freq.h"
#include "huffman/huffman.h"
#include "trivial/trivial.h"
#include "stream_tester/stream_tester.h"
#include "stream_tester/help.h"

int main(int argc, char const *argv[]) {
  time_t start = time(nullptr);
  std::string file_name("/tmp/data1");
  if (argc > 1) {
    std::string arg(argv[1]);
    if (arg == "--help" || arg == "-h") {
      std::cout << Help::help_message << std::endl;
      return 0;
    } else {
      file_name = arg;
    }
  }

  Codecs::HuffmanCodec huffman;
  Codecs::FreqCodec freq;
  Codecs::TrivialCodec triv;
  Codecs::MultiCodec main_codec(2, &freq, &huffman);
  StreamTester tester;
  tester.set_codec(huffman);
  tester.set_data_file(file_name);
  tester.learn_codec();
  tester.test_all();

  std::cout << "Program ended in " << time(nullptr) - start <<
    " seconds" << std::endl;
  return 0;
}
