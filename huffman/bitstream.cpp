#include <iostream>  // for debug
#include "huffman/bitstream.h"

bitstream::bitstream(const string_view& str, size_t sz)
  : data(&str)
  , size(sz)
  , ch(str[0])
  , iter(0)
  , index(0)
{}

bool bitstream::get_next() {
  bool res = ch & (1 << 7);
  // std::cout << "bs: " << ch << " -> " << res << std::endl;
  ++iter;
  ch = ch << 1;
  if (iter > 7 && !this->ended()) {
    ch = data->at(++index);
    iter = 0;
  }
  return res;
}

bool bitstream::ended() const {
  return iter + index * CHAR_SIZE >= size;
}
