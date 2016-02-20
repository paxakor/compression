#pragma once

#include <cstddef>
#include <string>
#include <vector>

using std::string;
using std::vector;

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

string save_bools(const vector<bool>&);
size_t load_int(string::iterator&, const string::iterator&);
vector<bool> load_bools(string::iterator&, const string::iterator&);
vector<string> split(const string&, size_t = 0, char = ' ');
