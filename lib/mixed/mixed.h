// Copyright 2016, Pavel Korozevtsev.

#pragma once

#include <cinttypes>
#include <functional>
#include <queue>
#include <string>
#include <utility>
#include <vector>
#include <experimental/string_view>
#include "lib/codec/codec.h"
#include "lib/common/defs.h"
#include "lib/mixed/tree.h"
#include "lib/mixed/trie.h"

namespace Codecs {

using Pair = std::pair<size_t, size_t>;
using Heap = std::priority_queue< Pair, vector<Pair>, std::greater<Pair> >;
using wide_string = std::basic_string<CharT>;
using wide_string_view = std::experimental::basic_string_view<CharT>;
using std::experimental::string_view;

class MixedCodec : public CodecIFace {
public:
  MixedCodec(const size_t);
  ~MixedCodec();

  void encode(string&, const string_view&) const override;
  void decode(string&, const string_view&) const override;

  string save() const override;
  void load(const string_view&) override;

  size_t sample_size(size_t) const override;
  void learn(const vector<string_view>&) override;

  void reset() override;

protected:
// from FreqCodec:
  void freq_encode(wide_string&, const string_view&) const;
  void freq_decode(string&, const wide_string_view&) const;
  void freq_learn(const vector<string_view>&);
  void build_trie();
  Trie::Trie trie;
  vector<string> strs;
  vector<string> strs_for_build;
  const size_t power;

// from HuffmanCodec:
  void huff_encode(string&, const wide_string_view&) const;
  void huff_decode(wide_string&, const string_view&) const;
  void precalc_frequency(const vector<wide_string_view>&);
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
  static constexpr size_t fr_sz = DICT_SIZE * sizeof(size_t);
};

}  // namespace Codecs
