cmake_minimum_required(VERSION 3.4)
project(tester)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Ofast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")  # flag for debug with GDB

include_directories("./" "tester/" "huffman/" "codec/")

set(SOURCES
    common/utils.cpp
    huffman/bitstream.cpp
    huffman/huffman.cpp
    huffman/tree.cpp
    tester/config.cpp
    tester/main.cpp
    tester/tester.cpp)

set(HEADERS
    codec/codec.h
    common/defs.h
    common/utils.h
    huffman/bitstream.h
    huffman/huffman.h
    huffman/tree.h
    tester/config.h
    tester/tester.h)

add_executable(tester ${SOURCES})
file(COPY tester/tester.conf DESTINATION ./)
