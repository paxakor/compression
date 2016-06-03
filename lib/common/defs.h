// Copyright 2016, Pavel Korozevtsev.

#pragma once

#include <cinttypes>
#include <climits>
#include <cstddef>

constexpr size_t CHAR_SIZE = sizeof(char) * CHAR_BIT;
constexpr size_t DICT_SIZE = static_cast<size_t>(1) << CHAR_SIZE;
