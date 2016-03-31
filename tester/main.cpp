#include <ctime>
#include <iostream>
#include "codec/multi_codec.h"
#include "common/utils.h"
#include "huffman/huffman.h"
#include "tester/config.h"
#include "tester/tester.h"

int main () {
  time_t start = time(nullptr);
  Config conf;
  conf.read();

  Codecs::HuffmanCodec huffman[3];
  Codecs::MultiCodec main_codec(3, huffman, huffman + 1, huffman + 2);
  Tester tester;
  tester.set_codec(main_codec);
  tester.read_data(conf["data_file"]);
  tester.learn_codec();
  tester.test_encode();

#if 1
  const auto state = main_codec.save();
  main_codec.reset();
  main_codec.load(state);
  tester.test_decode();
  tester.test_size();
  tester.test_correctness();
#endif

  std::cout << "Program ended in " << time(nullptr) - start <<
    " seconds" << std::endl;
  return 0;
}
