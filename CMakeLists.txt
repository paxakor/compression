cmake_minimum_required(VERSION 3.4)
project(tester)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Ofast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")  # flag for debug with GDB

include_directories("tester/" "huffman/" "codec/")

set(SOURCES
    main.cpp
    huffman/huffman.cpp
    tester/tester.cpp)

set(HEADERS
    codec/codec.h
    huffman/huffman.h
    tester/tester.h)

add_executable(tester ${SOURCES})
