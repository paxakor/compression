// Copyright 2016, Pavel Korozevtsev.

#include <cctype>
#include <iostream>
#include <string>
#include "testers/args_helper.h"

constexpr char help_message[] =
R"(Usage: tester [option] [FILE]
Options:
  -h, --help    print this message and exit
  -1 ... -9     compress faster(1) or better(9) (default = 9)
  -s            read strings (default)
  -b            read blocks: "LE uint32_t (size) + record")";

const Args parse(const int argc, char const *argv[]) {
  Args res;
  for (int i = 1; i < argc; ++i) {
    std::string arg(argv[i]);
    if (arg == "-h" || arg == "--help") {
      res.exit = -1;
      std::cout << help_message << std::endl;
      return res;
    } else if (arg[0] == '-' && isdigit(arg[1])) {
      res.power = std::stoi(arg.substr(1));
    } else if (arg == "-b") {
      res.read_block = true;
    } else if (arg == "-s") {
      res.read_block = false;
    } else if (i + 1 == argc) {
      res.file_name = arg;
    } else {
      std::cout << "wrong argument!" << std::endl;
      res.exit = 1;
      break;
    }
  }
  if (res.file_name == "") {
    std::cout << "empty file name!" << std::endl;
    res.exit = 1;
  }
  return res;
}
