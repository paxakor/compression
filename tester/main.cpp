#include <ctime>
#include <fstream>
#include <iostream>
#include "huffman/huffman.h"
#include "tester/config.h"
#include "tester/tester.h"

int main () {
  time_t start = time(nullptr);
  Config conf;
  conf.read();

  Codecs::HuffmanCodec huffman;
  Tester tester;
  tester.set_codec(huffman);
  tester.read_data(conf["data_file"]);
  tester.learn_codec();
  tester.test_encode();
  tester.test_decode();
  tester.test_size();
  tester.test_correctness();

  huffman.reset();
  std::cout << "Program ended in " << time(nullptr) - start <<
    " seconds" << std::endl;
  return 0;
}
