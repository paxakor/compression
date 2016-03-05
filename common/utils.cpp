#include <algorithm>
#include "common/defs.h"
#include "common/utils.h"

vector<string> split(const string& str, size_t count, char ch) {
  size_t pos = 0;
  size_t cnt = 0;
  vector<string> res;
  while (pos < str.size() && (count == 0 || cnt < count - 1)) {
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
  string str(1, 0);
  size_t iter = mv;
  char code = 0;
  for (const auto& bit : bools) {
    code = (code << 1) + bit;
    if (++iter == CHAR_SIZE) {
      str.push_back(code);
      code = 0;
      iter = 0;
    }
  }
  str.push_back(code << (CHAR_SIZE - iter));
  str[0] = static_cast<char>(iter);
  return str;
}

string bools_to_string_old(const vector<bool>& bools) {
  string str(1, 0);
  size_t i = 0;
  auto iter = bools.begin();
  while (iter != bools.end()) {
    char code = 0;
    for (i = 0; i < CHAR_SIZE && iter != bools.end(); ++i, ++iter) {
      code = (code << 1) + *iter;
    }
    code = code << (CHAR_SIZE - i);
    str.push_back(code);
  }
  str[0] = static_cast<char>(CHAR_SIZE - i);
  return str;
}

string print_string_binary(const string& str) {
  string result;
  for (const auto& ch : str) {
    result += print_number_binary(ch) + " ";
  }
  result.erase(result.size() - 1, 1);
  return result;
}
