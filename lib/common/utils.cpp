// Copyright 2016, Pavel Korozevtsev.

#include <algorithm>
#include <string>
#include <vector>
#include "lib/common/defs.h"
#include "lib/common/utils.h"

vector<string> split(const string& str, size_t count, char ch) {
  size_t pos = 0;
  size_t cnt = 0;
  vector<string> res;
  while (pos < str.size() && (count == 0 || cnt + 1 < count)) {
    size_t len = 0;
    while ((pos + len) < str.size() && str[pos + len] == ch) {
      ++pos;
    }
    while ((pos + len) < str.size() && str[pos + len] != ch) {
      ++len;
    }
    if (len > 0) {
      res.push_back(str.substr(pos, len));
    }
    pos += len + 1;
    ++cnt;
  }
  if (pos < str.size()) {
    res.push_back(str.substr(pos, str.size() - pos));
  }
  return res;
}

string bools_to_string(const vector<bool>& bools, size_t mv) {
  string str(1 + (mv / (sizeof(string::value_type) * CHAR_BIT)), 0);
  size_t iter = mv % CHAR_BIT;  // iter shows how many bits are already filled
  char code = 0;
  for (const auto& bit : bools) {
    code = (code << 1) + bit;
    if (++iter == CHAR_BIT) {
      str.push_back(code);
      code = 0;
      iter = 0;
    }
  }
  if (iter == 0) {
    str[0] = static_cast<char>(0);
  } else {
    str.push_back(code << (CHAR_BIT - iter));
    str[0] = static_cast<char>(iter);
  }
  return str;
}

void shrink_all_strings(vector<string>& vec) {
  for (auto& str : vec) {
    str.shrink_to_fit();
  }
}

string print_string_binary(const string& str) {
  string result;
  for (const auto& ch : str) {
    result += print_number_binary(ch) + " ";
  }
  result.erase(result.size() - 1, 1);
  return result;
}
