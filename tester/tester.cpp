#include <time.h>
#include <fstream>
#include <iostream>
#include "tester.h"

template <typename Iter>
void select_sample(StringViewVector& sample, Iter begin, Iter end,
  size_t sample_size) {
  std::experimental::sample(begin, end, std::back_inserter(sample),
    sample_size, std::mt19937(std::random_device()()));
}

void Tester::learn_codec() {
  std::cout << "Start learning" << std::endl;
  time_t start = time(nullptr);
  StringViewVector sample;
  select_sample(sample, this->data.begin(), this->data.end(),
    this->data.size() * 0.1);
  this->codec->learn(sample);
  std::cout << "Learning ended in " << time(nullptr) - start <<
    " seconds" << std::endl;
}

void Tester::read_data(const std::string& data_file) {
  std::cout << "Reading data from " << data_file << std::endl;
  std::ifstream input(data_file);
  while (input.good()) {
    std::string record;
    input >> record;
    this->data.push_back(record);
  }
  input.close();
  std::cout << "Read " << this->data.size() << " records" << std::endl;
}

void Tester::set_codec(Codecs::CodecIFace& codec) {
  *(this->codec) = codec;
}

void Tester::test_encode() {
  std::cout << "Start encoding" << std::endl;
  time_t start = time(nullptr);
  for (const auto& record : this->data) {
    std::experimental::string_view out;
    this->codec->encode(out, record);
    this->encoded.push_back(out);
  }
  std::cout << "Encoding ended in " << time(nullptr) - start <<
    " seconds" << std::endl;
}

void Tester::test_decode() {
  std::cout << "Start decoding" << std::endl;
  time_t start = time(nullptr);
  for (const auto& record : this->encoded) {
    std::experimental::string_view out;
    this->codec->decode(out, record);
    this->decoded.push_back(out);
  }
  std::cout << "Decoding ended in " << time(nullptr) - start <<
    " seconds" << std::endl;
}

void Tester::test_correctness() {
  std::cout << "Starting to check correctness" << std::endl;
  if (this->encoded.size() != this->decoded.size()) {
    std::cout << "Error: wrong data size" << std::endl;
    return;
  }
  auto enc_iter = this->encoded.begin();
  auto dec_iter = this->decoded.begin();
  size_t error_count = 0;
  while (enc_iter != this->encoded.end() && dec_iter != this->decoded.end()) {
    if (*enc_iter != *dec_iter) {
      ++error_count;
    }
    ++enc_iter;
    ++dec_iter;
  }
  float errors = 100.0 * error_count / this->encoded.size();
  std::cout << "Errors: " << errors << "%%" << std::endl;
}