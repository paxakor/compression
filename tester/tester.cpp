#include <ctime>
#include <experimental/algorithm>
#include <fstream>
#include <iostream>
#include "tester/tester.h"

template <typename Container, typename Iter>
void select_sample(Container& cont, Iter begin, Iter end, size_t sample_size) {
  std::experimental::sample(begin, end, std::back_inserter(cont),
    sample_size, std::mt19937(std::random_device()()));
}

void Tester::learn_codec() {
  std::cout << "Learning" << std::endl;
  time_t start = time(nullptr);
  vector<std::experimental::string_view> sample;
  select_sample(sample, this->data.begin(), this->data.end(),
    this->codec->sample_size(this->data.size()));
  std::cout << "Sample selected" << std::endl;
  this->codec->learn(sample);
  std::cout << "Learning ended in " << time(nullptr) - start <<
    " seconds" << std::endl;
}

void Tester::read_data(const string& data_file) {
  std::cout << "Reading data from " << data_file << std::endl;
  std::ifstream input(data_file);
  while (input.good()) {
    string record;
    std::getline(input, record);
    this->data.push_back(record);
  }
  input.close();
  std::cout << "Read " << this->data.size() << " records" << std::endl;
}

void Tester::save_encoded(const string& output_file) const {
  std::ofstream output(output_file);
  for (const auto& record : this->encoded) {
    output << record << std::endl;
  }
  output.clear();
}

void Tester::set_codec(Codecs::CodecIFace& codec) {
  this->codec = &codec;
}

void Tester::test_encode() {
  std::cout << "Start encoding" << std::endl;
  time_t start = time(nullptr);
  for (const auto& record : this->data) {
    string out;
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
    std::string out;
    this->codec->decode(out, record);
    this->decoded.push_back(out);
  }
  std::cout << "Decoding ended in " << time(nullptr) - start <<
    " seconds" << std::endl;
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
  std::cout << "Compressed on " << (1 - b / a) * 100 << "%" << std::endl;
}

size_t print_size(const vector<string>& vec, const string& name) {
  size_t sz = 1;
  for (const auto& s : vec) {
    sz += s.size() + 1;
  }
  const size_t bytes = sz;
  const size_t mod = 1024;
  const vector<string> unit_name = {"B", "KiB", "MiB", "GiB", "TiB", "PiB",
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
