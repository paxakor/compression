// Copyright 2016, Pavel Korozevtsev.

#pragma once

#include <cinttypes>
#include <climits>
#include <cstddef>

using CharT = char;
using UCharT = uint8_t;
constexpr size_t CHAR_SIZE = sizeof(CharT) * CHAR_BIT;
constexpr size_t DICT_SIZE = static_cast<size_t>(1) << 8;
