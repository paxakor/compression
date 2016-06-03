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
template <typename Int>
string print_number_binary(Int number) {
  constexpr size_t sz = sizeof(Int) * CHAR_BIT;
  string res;
  res.reserve(sz);
  for (size_t i = 0; i < sz; ++i) {
    if (number & (static_cast<Int>(1) << (sz - i - 1))) {
      res.push_back('1');
    } else {
      res.push_back('0');
    }
  }
  return res;
}

template <typename Str>
string print_string_binary(const Str& str) {
  string result;
  for (const auto& ch : str) {
    result += print_number_binary(ch) + " ";
  }
  result.erase(result.size() - 1, 1);
  return result;
}
