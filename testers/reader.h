#pragma once

#include <cinttypes>
#include <fstream>
#include <string>

class Reader {
public:
  Reader(const std::string& file_name, bool type)
    : _type(type)
    , _file(file_name, std::ios::binary) { }

  ssize_t get(std::string& str) {
    if (!_file.good()) {
      return -1;
    }
    if (_type) {
      std::getline(_file, str);
    } else {
      uint32_t sz = 0;
      _file.read(reinterpret_cast<char*>(&sz), sizeof(sz));
      str.resize(sz);
      _file.read(const_cast<char*>(str.data()), sz);
    }
    return str.size();
  }

  bool good() const {
    return _file.good();
  }

  void close() {
    _file.close();
  }

private:
  const bool _type;
  std::ifstream _file;
};
