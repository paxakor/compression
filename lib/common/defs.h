// Copyright 2016, Pavel Korozevtsev.

#pragma once

#include <climits>
#include <cstddef>

const size_t CHAR_SIZE = sizeof(char) * CHAR_BIT;
const size_t my256 = static_cast<size_t>(1) << CHAR_SIZE;
