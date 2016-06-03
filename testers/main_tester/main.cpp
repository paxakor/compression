// Copyright 2016, Pavel Korozevtsev.

#include "lib/mixed/mixed.h"
#include "testers/args_helper.h"
#include "testers/main_tester/tester.h"
#include "testers/stopwatch.h"

int main(int argc, char const* argv[]) {
  auto args = parse(argc, argv);
  if (args.exit) {
    return args.exit + 1;
  }
  Stopwatch sw("Program");
  Codecs::MixedCodec mix(args.power);
  Tester tester;
  tester.set_codec(mix);
  tester.read_data(args.file_name, args.read_block);
  tester.learn_codec();
  tester.test_encode();
#if 1
  const auto state = mix.save();
  mix.reset();
  mix.load(state);
#endif
#if 1
  tester.test_decode();
  tester.test_size();
  tester.test_correctness();
#endif
  return 0;
}
