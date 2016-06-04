// Copyright 2016, Pavel Korozevtsev.

#pragma once

#include <string>

using std::string;

struct Args {
  int exit = 0;
  bool read_block = false;
  size_t power = 9;
  string file_name;
};

const Args parse(int argc, char const *argv[]);
