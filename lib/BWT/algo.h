#include <algorithm>
#include <string>
#include <vector>
#include "common/defs.h"

using std::pair;
using std::string;
using std::vector;

size_t char_to_size_t(char ch) {
  return static_cast<unsigned char>(ch);
}

template <typename SomeStr>
vector<size_t> counting_sort(const SomeStr& str) {
  vector< vector<size_t> > counts(my256);
  size_t min = 0, max = 0;
  for (size_t i = 0; i < str.size(); ++i) {
    size_t t = char_to_size_t(str[i]);
    counts[t].push_back(i);
    min = std::min(min, t);
    max = std::max(max, t);
  }
  vector<size_t> res;
  do {
    if (!counts[min].empty()) {
      res.insert(res.end(), counts[min].cbegin(), counts[min].cend());
    }
  } while (min++ != max);
  return res;
}

template <typename SomeStr>
vector<size_t> suff_mas(const SomeStr& str) {
  const auto n = str.size();
  auto permutation = counting_sort(str);
  vector< pair<size_t, size_t> > classes(n);
  for (size_t i = 0; i < n; ++i) {
    classes[i] = {char_to_size_t(str[i]), 0};
  }
  for (size_t LEN = 1; LEN < n; LEN *= 2) {
    vector<size_t> colours(n);
    size_t clr = 0;
    auto ch = classes[permutation[0]];
    for (size_t i = 0; i < n; ++i) {
      auto j = permutation[i];
      if (classes[j] != ch) {
        ++clr;
        ch = classes[j];
      }
      colours[j] = clr;
    }

    vector< pair< pair<size_t, size_t>, size_t > > new_classes;
    for (size_t i = 0; i < n; ++i) {
      new_classes.push_back({{colours[i], colours[(i + LEN) % n]}, i});
    }
    std::sort(new_classes.begin(), new_classes.end());
    for (size_t i = 0; i < n; ++i) {
      const auto& pp = new_classes[i];
      permutation[i] = pp.second;
      classes[permutation[i]] = pp.first;
    }
  }
  return permutation;
}
