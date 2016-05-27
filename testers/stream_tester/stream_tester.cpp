// Copyright 2016, Pavel Korozevtsev.

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <experimental/string_view>
#include "testers/stream_tester/stream_tester.h"
#include "testers/reader.h"
#include "testers/stopwatch.h"

using std::string;
using std::vector;

size_t get_file_size(const string& file_name) {
  std::ifstream file(file_name, std::ios::binary);
  const auto begin = file.tellg();
  file.seekg(0, std::ios::end);
  const auto end = file.tellg();
  return end - begin;
}

size_t get_records_count(const string& file_name, bool read_block) {
  auto data_size = get_file_size(file_name);
  vector<string> sample_storage;
  Reader input(file_name, read_block);
  while (sample_storage.size() < 128 && input.good()) {
    string record;
    input.get(record);
    sample_storage.push_back(record);
  }
  input.close();
  size_t record_size = 0;
  for (const auto rec : sample_storage) {
    record_size += rec.size();
  }
  record_size /= sample_storage.size();
  return data_size / record_size;
}

size_t print_size(size_t sz, const string& name) {
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

void StreamTester::set_data_file(const string& new_data_file, bool type) {
  this->data_file = new_data_file;
  this->read_block = type;
}

void StreamTester::learn_codec() {
  std::cout << "Reading sample from " << this->data_file << std::endl;
  size_t records_count = get_records_count(this->data_file, this->read_block);
  Reader input(this->data_file, this->read_block);
  const size_t smpl_sz = this->codec->sample_size(records_count);
  vector<string> sample_storage;
  size_t k = records_count / smpl_sz;
  for (size_t i = 0; sample_storage.size() < smpl_sz && input.good(); ++i) {
    string record;
    input.get(record);
    if (i % k == 0) {
      sample_storage.push_back(record);
    }
  }
  input.close();
  vector<std::experimental::string_view> sample;
  for (const auto rec : sample_storage) {
    sample.push_back(rec);
  }
  std::cout << "Read " << sample.size() << " records" << std::endl;
  Stopwatch sw("Learning");
  this->codec->learn(sample);
}

void StreamTester::set_codec(Codecs::CodecIFace& codec) {
  this->codec = &codec;
}

void StreamTester::test_all() const {
  Stopwatch sw("Testing");
  std::ifstream file(this->data_file, std::ios::binary);
  size_t data_size = 0;
  size_t encoded_size = 0;
  size_t errors = 0;
  size_t total = 0;
  while (file.good()) {
    string record, encoded, decoded;
    std::getline(file, record);
    this->codec->encode(encoded, record);
    this->codec->decode(decoded, encoded);
    if (decoded != record) {
      ++errors;
    }
    data_size += record.size();
    encoded_size += encoded.size();
    ++total;
  }
  std::cout << "Encoded " << total << " records" << std::endl;
  const float a = print_size(data_size,    "data   ");
  const float b = print_size(encoded_size, "encoded");
  std::cout << "Compressed by " << (1 - b / a) * 100 << "%" << std::endl;
  std::cout << "Errors: " << 100.0 * errors / total << "%" << std::endl;
}
