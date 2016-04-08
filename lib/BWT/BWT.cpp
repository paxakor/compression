#include <cstring>
#include <algorithm>
#include <vector>
#include <string>
#include "BWT/BWT.h"
#include "BWT/algo.h"

namespace Codecs {

void BWTCodec::encode(string& encoded, const string_view& raw) const {
  size_t num;
  const auto sz = sizeof(num);
  const auto n = raw.size();
  const auto permutation = suff_mas(raw);
  encoded.reserve(n + sz);
  for (size_t i = 0; i < n; ++i) {
    const auto id = permutation[i];
    if (id == 0) {
      num = i;
      encoded.push_back(raw[n - 1]);
    } else {
      encoded.push_back(raw[id - 1]);
    }
  }
  char* tmp = new char[sz];
  memcpy(tmp, &num, sz);
  encoded.insert(encoded.end(), tmp, tmp + sz);
  delete[] tmp;
}

void BWTCodec::decode(string& raw, const string_view& encoded) const {
  size_t num;
  const auto sz = sizeof(num);
  const auto n = encoded.size() - sz;
  memcpy(&num, &encoded[n], sz);
  vector<string> table(n);
  for (auto& str : table) {
    str.resize(n);
  }
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      table[j][n - i - 1] = encoded[j];
    }
    std::sort(table.begin(), table.end());
  }
  raw = table[num];
}

string BWTCodec::save() const {
  return string();
}

void BWTCodec::load(const string_view& config) { }

size_t BWTCodec::sample_size(size_t records_total) const {
  return 0;
}

void BWTCodec::learn(const vector<string_view>& all_samples) { }

void BWTCodec::reset() { }

} // namespace Codecs
