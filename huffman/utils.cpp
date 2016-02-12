#include "utils.h"

string save_vec_bool(const std::vector<bool>& vec) {
  string res;
  auto iter = vec.begin();
  const size_t sz = vec.size() / sizeof(char);
  res += static_cast<char>(sz);
  for (size_t i = 0; i < sz; ++i) {
    char c = 0;
    for (size_t j = 0; j < sizeof(char); ++j) {
      bool b = 0;
      if (iter != vec.end()) {
        b = *(iter++);
      }
      c = (c << 1) + b;
    }
    res += c;
  }
  return res;
}

size_t load_int(string::iterator& iter, const string::iterator& end) {
  if (iter == end) {
    return 0;
  }
  const size_t sz = *(iter++);
  const size_t mod = (static_cast<size_t>(1) << sizeof(char));
  size_t res = 0;
  for (size_t i = 0; i < sz && iter != end; ++i) {
    res = (res << mod) + *(iter++);
  }
  return res;
}
