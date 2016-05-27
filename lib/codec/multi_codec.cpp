// Copyright 2016, Pavel Korozevtsev.

#include <cstdarg>
#include <cstring>
#include <algorithm>
#include <string>
#include <vector>
#include "lib/codec/multi_codec.h"

namespace Codecs {

MultiCodec::MultiCodec(size_t count, CodecIFace* fst_cdc, ...) {
  this->push(fst_cdc);
  va_list args;
  va_start(args, fst_cdc);
  for (size_t i = 0; i + 1 < count; ++i) {
    this->push(va_arg(args, CodecIFace*));
  }
  va_end(args);
}

void MultiCodec::push(CodecIFace* cdc_prt) {
  this->codecs_list.push_back(cdc_prt);
}

void MultiCodec::pop() {
  this->codecs_list.pop_back();
}

void MultiCodec::encode(string& encoded, const string_view& raw) const {
  const auto codecs_count = this->codecs_list.size();
  auto cdc = this->codecs_list.begin();
  if (codecs_count == 1) {
    (*cdc)->encode(encoded, raw);
  } else {
    string* const tmp_str = new string[codecs_count - 1];
    (*(cdc++))->encode(tmp_str[0], raw);
    for (size_t i = 0; i + 2 < codecs_count; ++i) {
      (*(cdc++))->encode(tmp_str[i + 1], tmp_str[i]);
    }
    (*cdc)->encode(encoded, tmp_str[codecs_count - 2]);
    delete[] tmp_str;
  }
}

void MultiCodec::decode(string& raw, const string_view& encoded) const {
  const auto codecs_count = this->codecs_list.size();
  auto cdc = this->codecs_list.rbegin();
  if (codecs_count == 1) {
    (*cdc)->decode(raw, encoded);
  } else {
    string* const tmp_str = new string[codecs_count - 1];
    (*(cdc++))->decode(tmp_str[0], encoded);
    for (size_t i = 0; i + 2 < codecs_count; ++i) {
      (*(cdc++))->decode(tmp_str[i + 1], tmp_str[i]);
    }
    (*cdc)->decode(raw, tmp_str[codecs_count - 2]);
    delete[] tmp_str;
  }
}

string MultiCodec::save() const {
  constexpr auto num_sz = sizeof(size_t);
  string data;
  for_each_codec([&](CodecIFace* cdc) {
    const auto saved = cdc->save();
    const size_t saved_size = saved.size();
    char len[num_sz];
    memcpy(len, &saved_size, num_sz);
    data.insert(data.end(), len, len + num_sz);
    if (saved_size > 0) {
      data.insert(data.end(), saved.begin(), saved.end());
    }
  });
  return data;
}

void MultiCodec::load(const string_view& config) {
  constexpr auto num_sz = sizeof(size_t);
  size_t already_read = 0;
  for_each_codec([&](CodecIFace* cdc) {
    if (already_read + num_sz < config.size()) {
      size_t new_size = 0;
      memcpy(&new_size, &config[already_read], num_sz);
      already_read += num_sz;
      cdc->load(config.substr(already_read, new_size));
      already_read += new_size;
    }
  });
}

void MultiCodec::learn(const vector<string_view>& all_samples) {
  vector<string> samples;
  for (const auto& smpl : all_samples) {
    samples.emplace_back(smpl.to_string());
  }
  for_each_codec([&](CodecIFace* cdc) {
    cdc->learn(vector<string_view>(samples.begin(), samples.end()));
    for (auto& smpl : samples) {
      string tmp;
      cdc->encode(tmp, smpl);
      smpl = tmp;
    }
  });
}

size_t MultiCodec::sample_size(size_t records_total) const {
  size_t max = 0;
  for_each_codec([&](const CodecIFace* cdc) {
    max = std::max(max, cdc->sample_size(records_total));
  });
  return max;
}

void MultiCodec::reset() {
  for_each_codec([](CodecIFace* cdc) {
    cdc->reset();
  });
}

}  // namespace Codecs
