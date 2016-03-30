#include <cstdarg>
#include <cstring>
#include <vector>
#include <string>
#include "codec/multi_codec.h"

namespace Codecs {

MultiCodec::MultiCodec() { }

MultiCodec::MultiCodec(size_t count, ...) {
  va_list args;
  va_start(args, count);
  for (size_t i = 0; i < count; ++i) {
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

void MultiCodec::encode(string& encoded, const string_view& raw) const {}

void MultiCodec::decode(string& raw, const string_view& encoded) const {}

string MultiCodec::save() const {
  static const auto cnt = sizeof(size_t);
  string data;
  for_each_codec([&](CodecIFace* cdc){
    const auto tmp = cdc->save();
    const size_t saved_size = tmp.size();
    char len[cnt];
    memcpy(len, &saved_size, cnt);
    data.insert(data.end(), len, len + cnt);
    data.insert(data.end(), tmp.begin(), tmp.end());
  });
  return data;
}

void MultiCodec::load(const string_view& config) {
  static const auto cnt = sizeof(size_t);
  size_t already_read = 0;
  for_each_codec([&](CodecIFace* cdc){
    size_t new_size;
    memcpy(&new_size, &config[already_read], cnt);
    already_read += cnt;
    cdc->load(config.substr(already_read, new_size));
    already_read += new_size;
  });
}

void MultiCodec::learn(const vector<string_view>& all_samples) {
  for_each_codec([&](CodecIFace* cdc){
    cdc->learn(all_samples);
  });
}

size_t MultiCodec::sample_size(size_t records_total) const {
  size_t max = 0;
  for_each_codec([&](const CodecIFace* cdc){
    max = std::max(max, cdc->sample_size(records_total));
  });
  return max;
}

void MultiCodec::reset() {
  for_each_codec([](CodecIFace* cdc){
    cdc->reset();
  });
}

} // namespace Codecs
