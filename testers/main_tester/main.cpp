// Copyright 2016, Pavel Korozevtsev.

#include "lib/codec/multi_codec.h"
#include "lib/freq/freq.h"
#include "lib/huffman/huffman.h"
#include "lib/trivial/trivial.h"
#include "testers/args_helper.h"
#include "testers/main_tester/tester.h"
#include "testers/stopwatch.h"

int main(int argc, char const *argv[]) {
  auto args = parse(argc, argv);
  if (args.exit) {
    return args.exit + 1;
  }
  Stopwatch sw("Program");
  Codecs::HuffmanCodec huffman;
  Codecs::FreqCodec freq(args.power);
  Codecs::TrivialCodec triv;
  Codecs::MultiCodec main_codec(2, &freq, &huffman);
  Tester tester;
  tester.set_codec(main_codec);
  tester.read_data(args.file_name, args.read_block);
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
