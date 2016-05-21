// Copyright 2016, Pavel Korozevtsev.

#include <fstream>
#include <string>
#include "tester/config.h"
#include "common/utils.h"

Config::Config() {}

Config::Config(const std::string& file_name) {
  this->read(file_name);
}

void Config::read(const std::string& file_name) {
  std::ifstream conf_file(file_name);
  std::string str;
  while (conf_file.good()) {
    std::getline(conf_file, str);
    const auto vec = split(str, 2);
    if (vec.size() < 2) {
      continue;
    }
    this->insert({vec[0], vec[1]});
  }
  conf_file.close();
}
