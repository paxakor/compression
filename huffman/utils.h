#pragma once

#include <stddef.h>
#include <string>
#include <vector>

using std::string;

template <typename Int>
string save_int(Int number) {
  const size_t sz = sizeof(number);
  const Int mod = (static_cast<size_t>(1) << sizeof(char));
  string str;
  str.push_back(static_cast<char>(sz));
  for (size_t i = 0; i < sz; ++i) {
    str.push_back(number % mod);
    number /= mod;
  }
  return str;
}

string save_vec_bool(const std::vector<bool>&);

size_t load_int(string::iterator&, const string::iterator&);
