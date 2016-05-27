// Copyright 2016, Pavel Korozevtsev.

#include "lib/codec/multi_codec.h"
#include "lib/freq/freq.h"
#include "lib/huffman/huffman.h"
#include "lib/trivial/trivial.h"
#include "testers/args_helper.h"
#include "testers/stopwatch.h"
#include "testers/stream_tester/stream_tester.h"

int main(int argc, char const* argv[]) {
  Stopwatch sw("Program");
  auto args = parse(argc, argv);
  if (args.exit) {
    return args.exit + 1;
  }
  Codecs::HuffmanCodec huffman;
  Codecs::FreqCodec freq(args.power);
  Codecs::TrivialCodec triv;
  Codecs::MultiCodec main_codec(2, &freq, &huffman);
  StreamTester tester;
  tester.set_codec(main_codec);
  tester.set_data_file(args.file_name, args.read_block);
  tester.learn_codec();
  tester.test_all();
  return 0;
}
