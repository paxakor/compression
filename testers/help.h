#pragma once

#include <string>

namespace Help {

using std::string;

const string help_message(const string& name) {
  string msg = 
R"(Usage: )" + name + R"( [option] [FILE]
Options:
  --help      print this message and exit
  FILE        read records from FILE)";
  return msg;
}

}
