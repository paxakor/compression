#include <time.h>
#include <iostream>
#include "huffman.h"
#include "tester.h"

int main () {
  time_t start = time(nullptr);
  Tester tester;
  Codecs::HuffmanCodec huffman;
  tester.set_codec(huffman);
  tester.read_data("../data/data1");
  tester.learn_codec();
  tester.test_encode();
  tester.test_decode();
  tester.test_correctness();
  std::cout << "Program ended in " << time(nullptr) - start <<
    " seconds" << std::endl;
  return 0;
}
