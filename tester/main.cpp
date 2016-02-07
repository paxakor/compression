#include "huffman.h"
#include "tester.h"

int main () {
  Tester tester;
  Codecs::HuffmanCodec huffman;
  tester.set_codec(huffman);
  tester.read_data("../data/data4");
  tester.learn_codec();
  tester.test_encode();
  tester.test_decode();
  tester.test_correctness();
  return 0;
}
