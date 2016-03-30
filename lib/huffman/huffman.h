#pragma once

#include <cinttypes>
#include <queue>
#include <unordered_map>
#include "codec/codec.h"
#include "common/defs.h"
#include "huffman/tree.h"

namespace Codecs {

using Pair = std::pair<size_t, size_t>;
using SmallPair = std::pair<uint8_t, uint16_t>;
using Heap = std::priority_queue< Pair, vector<Pair>, std::greater<Pair> >;

class HuffmanCodec : public CodecIFace {
public:
  HuffmanCodec();
  ~HuffmanCodec();

  void encode(string&, const string_view&) const override;
  void decode(string&, const string_view&) const override;

  string save() const override;
  void load(const string_view&) override;

  size_t sample_size(size_t) const override;
  void learn(const vector<string_view>&) override;

  void reset() override;

protected:
  void precalc_frequency(const vector<string_view>&);
  void load_frequency(const string_view&);
  void learn_or_load_all();
  Heap build_heap();
  void build_tree(Heap&);
  void build_table();
  void find_all_ways();

  Tree tree;
  string** table;
  SmallPair** tree_table;
  size_t* frequency;
  static const size_t fr_sz = my256 * sizeof(size_t);
};

}  // namespace Codecs