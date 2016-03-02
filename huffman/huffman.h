#pragma once

#include <unordered_map>
#include <queue>
#include "codec/codec.h"
#include "huffman/tree.h"

namespace Codecs {

using Pair = std::pair< size_t, size_t >;
using Heap = std::priority_queue< Pair, vector<Pair>, std::greater<Pair> >;

class HuffmanCodec : public CodecIFace {
public:
  void encode(string&, const string_view&) const override;
  void decode(string&, const string_view&) const override;

  string save() const override;
  void load(const string_view&) override;

  size_t sample_size(size_t) const override;
  void learn(const vector<string_view>&) override;

  void reset() override;
protected:
  Heap precalc(const vector<string_view>&);
  void build_tree(Heap&);
  void build_table();

  Tree tree;
  std::unordered_map< char, vector<bool> > table;
  char rare_char;
};

}  // namespace Codecs
