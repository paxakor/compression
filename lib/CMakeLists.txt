cmake_minimum_required(VERSION 3.0)
project(codec)

# add flags to CMAKE_CXX_FLAGS
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(functions)
add_compile_flags(CMAKE_CXX_FLAGS)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/codec
                    ${CMAKE_CURRENT_SOURCE_DIR}/common
                    ${CMAKE_CURRENT_SOURCE_DIR}/huffman)

set(SOURCES
  common/utils.cpp
  huffman/huffman.cpp
  huffman/tree.cpp)

add_library(codec SHARED ${SOURCES})
