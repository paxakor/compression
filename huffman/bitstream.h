#pragma once

#include <cstddef>
#include <experimental/string_view>
#include "common/defs.h"

using std::experimental::string_view;

class bitstream {
public:
  bitstream(const string_view&, size_t);
  bool get_next();
  bool ended() const;
private:
  const string_view* const data;
  const size_t size;
  char ch;
  size_t iter;
  size_t index;
};
