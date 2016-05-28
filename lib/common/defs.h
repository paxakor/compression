// Copyright 2016, Pavel Korozevtsev.

#pragma once

#include <climits>
#include <cstddef>

using CharT = char;
const size_t CHAR_SIZE = sizeof(CharT) * CHAR_BIT;
const size_t my256 = static_cast<size_t>(1) << CHAR_SIZE;
