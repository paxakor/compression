cmake_minimum_required(VERSION 3.0)
project(codec)

# add flags to CMAKE_CXX_FLAGS
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(functions)
add_compile_flags(CMAKE_CXX_FLAGS)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCES
  codec/multi_codec.cpp
  common/utils.cpp
  freq/freq.cpp
  freq/trie.cpp
  huffman/huffman.cpp
  huffman/tree.cpp
  mixed/mixed.cpp
  trivial/trivial.cpp)

add_library(codec SHARED ${SOURCES})
