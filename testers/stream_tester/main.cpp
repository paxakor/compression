// Copyright 2016, Pavel Korozevtsev.

#include <string>
#include "lib/codec/multi_codec.h"
#include "lib/freq/freq.h"
#include "lib/huffman/huffman.h"
#include "lib/trivial/trivial.h"
#include "testers/help.h"
#include "testers/stopwatch.h"
#include "testers/stream_tester/stream_tester.h"

int main(int argc, char const* argv[]) {
  Stopwatch sw("Program");
  std::string file_name("/tmp/data1");
  if (argc > 1) {
    std::string arg(argv[1]);
    if (arg == "--help" || arg == "-h") {
      std::cout << Help::help_message("stream_tester") << std::endl;
      return 0;
    }
    file_name = arg;
  }
  Codecs::HuffmanCodec huffman;
  Codecs::FreqCodec freq;
  Codecs::TrivialCodec triv;
  Codecs::MultiCodec main_codec(2, &freq, &huffman);
  StreamTester tester;
  tester.set_codec(main_codec);
  tester.set_data_file(file_name);
  tester.learn_codec();
  tester.test_all();
  return 0;
}
