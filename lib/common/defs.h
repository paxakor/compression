// Copyright 2016, Pavel Korozevtsev.

#pragma once

#include <cinttypes>
#include <climits>
#include <cstddef>

using CharT = uint16_t;
using UCharT = uint16_t;
constexpr size_t CHAR_SIZE = sizeof(char) * CHAR_BIT;
constexpr size_t WCHAR_SIZE = 12;
constexpr size_t DICT_SIZE = static_cast<size_t>(1) << WCHAR_SIZE;
constexpr UCharT MASK = (static_cast<UCharT>(1) << WCHAR_SIZE) - 1;
