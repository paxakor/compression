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
  std::cout << "start learning" << std::endl;
  time_t start = time(nullptr);
  StringViewVector sample;
  select_sample(sample, this->data.begin(), this->data.end(),
    this->data.size() * 0.1);
  this->codec->learn(sample);
  std::cout << "learning ended in " << time(nullptr) - start <<
    " seconds" << std::endl;
}

void Tester::read_data(const std::string& data_file) {
  std::ifstream input(data_file);
  while (input.good()) {
    std::string record;
    input >> record;
    this->data.push_back(record);
  }
  input.close();
}

void Tester::set_codec(Codecs::CodecIFace& codec) {
  *(this->codec) = codec;
}

void Tester::test_encode() {
  for (const auto& record : this->data) {
    std::experimental::string_view out;
    this->codec->encode(out, record);
    this->encoded.push_back(out);
  }
}

void Tester::test_decode() {
  for (const auto& record : this->encoded) {
    std::experimental::string_view out;
    this->codec->decode(out, record);
    this->decoded.push_back(out);
  }
}

void Tester::test_correctness() {
  if (this->encoded.size() != this->decoded.size()) {
    std::cout << "Error: wrong data size" << std::endl;
    return;
  }
  auto enc_iter = this->encoded.begin();
  auto dec_iter = this->decoded.begin();
  size_t error_count = 0;
  size_t data_count = this->encoded.size();
  while (enc_iter != this->encoded.end() && dec_iter != this->decoded.end()) {
    if (*enc_iter != *dec_iter) {
      ++error_count;
    }
    ++enc_iter;
    ++dec_iter;
  }
  float errors = 100.0 * error_count / data_count;
  std::cout << "Errors: " << errors << "%%" << std::endl;
}

