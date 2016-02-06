#include "tester.h"
#include <fstream>

void Tester::load_codec(Codecs::CodecIFace& codec) {
  *(this->codec) = codec;
}

void Tester::load_data(const std::string& data_file) {
  std::ifstream input(data_file);
  while (input.good()) {
    std::string record;
    input >> record;
    this->data.push_back(record);
  }
  input.close();
}

void Tester::test_encode() {

}

void Tester::test_decode() {

}

void Tester::test_correctness() {

}

