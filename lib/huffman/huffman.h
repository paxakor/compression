// Copyright 2016, Pavel Korozevtsev.

#pragma once

#include <cinttypes>
#include <functional>
#include <queue>
#include <string>
#include <utility>
#include <vector>
#include "lib/codec/codec.h"
#include "lib/common/defs.h"
#include "lib/huffman/tree.h"

namespace Codecs {

using Pair = std::pair<size_t, size_t>;
using Heap = std::priority_queue< Pair, vector<Pair>, std::greater<Pair> >;

class HuffmanCodec : public CodecIFace {
public:
  HuffmanCodec();
  ~HuffmanCodec();

  void encode(string&, const string&) const override;
  void decode(string&, const string&) const override;

  string save() const override;
  void load(const string&) override;

  size_t sample_size(size_t) const override;
  void learn(const vector<string>&) override;

  void reset() override;

protected:
  void precalc_frequency(const vector<string>&);
  void load_frequency(const string&);
  void learn_or_load_all();
  Heap build_heap();
  void build_tree(Heap&);
  void build_table();
  void find_all_ways();

  Tree tree;
  string** table;
  SmallPair** tree_table;
  size_t* frequency;
  static constexpr size_t fr_sz = DICT_SIZE * sizeof(size_t);
};

}  // namespace Codecs
