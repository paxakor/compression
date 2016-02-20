#include "common/defs.h"
#include "common/utils.h"

string save_bools(const vector<bool>& vec) {
  string res;
  auto iter = vec.begin();
  const auto end = vec.end();
  const size_t sz = vec.size() / CHAR_SIZE;
  res += static_cast<char>(sz - 1);
  res += static_cast<char>(vec.size() - (sz * CHAR_SIZE));
  for (size_t i = 0; i < sz; ++i) {
    char c = 0;
    for (size_t j = 0; j < CHAR_SIZE; ++j) {
      bool b = (iter != end) ? *(iter++) : 0;
      c = (c << 1) + b;
    }
    res += c;
  }
  return res;
}

size_t load_int(string::iterator& iter, const string::iterator& end) {
  const size_t sz = (iter != end) ? *(iter++) : 0;
  const size_t mod = (static_cast<size_t>(1) << CHAR_SIZE);
  size_t res = 0;
  for (size_t i = 0; i < sz && iter != end; ++i) {
    res = (res << mod) + *(iter++);
  }
  return res;
}

vector<bool> load_bools(string::iterator& iter, const string::iterator& end) {
  vector<bool> vec;
  const size_t sz = (iter != end) ? *(iter++) : 0;
  const size_t rest = (iter != end) ? *(iter++) : 0;
  for (size_t i = 0; i < sz && iter != end; ++i) {
    char c = *(iter++);
    for (size_t j = 0; j < CHAR_SIZE; ++j) {
      vec.push_back(c & 0x80);
      c = c << 1;
    }
  }
  char c = (iter != end) ? *(iter++) : 0;
  for (size_t j = 0; j < rest; ++j) {
    vec.push_back(c & 0x80);
    c = c << 1;
  }
  return vec;
}

vector<string> split(const string& str, size_t count, char ch) {
  size_t pos = 0;
  size_t cnt = 0;
  vector<string> res;
  while (pos < str.size() && (count == 0 || cnt < count - 1)) {
    size_t len = 0;
    while (str[pos + len] == ch) {
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
