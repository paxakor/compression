cmake_minimum_required(VERSION 3.4)
project(tester)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Ofast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")  # flag for debug with GDB

include_directories("tester/" "huffman/")

set(SOURCES
    main.cpp
    huffman/codec.cpp
    tester/tester.cpp)

set(HEADERS
    huffman/codec.h
    tester/tester.h)

add_executable(tester ${SOURCES})
