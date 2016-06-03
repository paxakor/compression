// Copyright 2016, Pavel Korozevtsev.

#include <iostream>
#include <string>
#include <vector>
#include <experimental/algorithm>
#include <experimental/string_view>
#include "lib/common/utils.h"
#include "testers/main_tester/tester.h"
#include "testers/reader.h"
#include "testers/stopwatch.h"

template <typename Container, typename Source>
void select_sample(Container& cont, const Source& src, size_t sample_size) {
  std::experimental::sample(src.begin(), src.end(), std::back_inserter(cont),
    sample_size, std::mt19937(std::random_device()()));
}

void Tester::learn_codec() {
  Stopwatch sw("Learning");
  std::vector<std::experimental::string_view> sample;
  const size_t smpl_sz = this->codec->sample_size(this->data.size());
  select_sample(sample, this->data, smpl_sz);
  this->codec->learn(sample);
}

void Tester::read_data(const string& data_file, bool type) {
  std::cout << "Reading data from " << data_file << std::endl;
  Reader input(data_file, type);
  while (input.good()) {
    string record;
    input.get(record);
    this->data.push_back(record);
  }
  input.close();
  std::cout << "Read " << this->data.size() << " records" << std::endl;
  shrink_all_strings(this->data);
}

void Tester::set_codec(Codecs::CodecIFace& codec) {
  this->codec = &codec;
}

void Tester::test_encode() {
  this->encoded.resize(this->data.size());
  auto iter = this->encoded.begin();
  Stopwatch sw("Encoding");
  for (const auto& record : this->data) {
    this->codec->encode(*iter, record);
    ++iter;
  }
  sw.end();
  shrink_all_strings(this->encoded);
}

void Tester::test_decode() {
  this->decoded.resize(this->data.size());
  auto iter = this->decoded.begin();
  Stopwatch sw("Decoding");
  for (const auto& record : this->encoded) {
    this->codec->decode(*iter, record);
    ++iter;
  }
  sw.end();
  shrink_all_strings(this->decoded);
}

void Tester::test_correctness() const {
  std::cout << "Starting to check correctness" << std::endl;
  if (this->data.size() != this->decoded.size()) {
    std::cout << "Error: wrong data size" << std::endl;
    return;
  }
  auto dat_iter = this->data.begin();
  auto dec_iter = this->decoded.begin();
  size_t error_count = 0;
  size_t total = this->data.size();
  while (dat_iter != this->data.end() && dec_iter != this->decoded.end()) {
    if (*dat_iter != *dec_iter) {
      ++error_count;
    }
    ++dat_iter;
    ++dec_iter;
  }
  float errors = 100.0 * error_count / total;
  std::cout << "Checked " << total << " records" << std::endl;
  std::cout << "Errors: " << errors << "%" << std::endl;
}

void Tester::test_size() const {
  const float a = print_size(this->data,    "data   ");
  const float b = print_size(this->encoded, "encoded");
  const float c = print_size(this->decoded, "decoded");
  std::cout << "Compressed by " << (1 - b / a) * 100 << "%" << std::endl;
}

size_t print_size(const std::vector<string>& vec, const string& name) {
  size_t sz = 0;
  for (const auto& s : vec) {
    sz += s.size();
  }
  sz *= sizeof(char);
  const size_t bytes = sz;
  const size_t mod = 1024;
  const std::vector<string> unit_name = {"B", "KiB", "MiB", "GiB", "TiB", "PiB",
    "EiB", "ZiB", "YiB"};
  size_t i = 0;
  while (sz > mod) {
    sz /= mod;
    ++i;
  }
  std::cout << "Size of " << name << " = " <<
    sz << " " << unit_name[i] << std::endl;
  return bytes;
}
