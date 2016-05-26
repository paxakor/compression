// Copyright 2016, Pavel Korozevtsev.

#pragma once

#include <climits>
#include <cstddef>
#include <string>
#include <vector>

using std::string;
using std::vector;

vector<string> split(const string&, size_t = 0, char = ' ');
string bools_to_string(const vector<bool>&, size_t = 0);
void shrink_all_strings(vector<string>&);

// utils for debug
string print_string_binary(const string&);

template <typename Int>
string print_number_binary(Int number) {
  string res;
  const size_t sz = sizeof(number) * CHAR_BIT;
  for (size_t i = 0; i < sz; ++i) {
    if (number & (static_cast<Int>(1) << (sz - i - 1))) {
      res.push_back('1');
    } else {
      res.push_back('0');
    }
  }
  return res;
}
