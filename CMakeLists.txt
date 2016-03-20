cmake_minimum_required(VERSION 3.4)
project(tester)

include_directories("./" "tester/" "huffman/" "codec/")

set(SOURCES
    common/utils.cpp
    huffman/huffman.cpp
    huffman/tree.cpp
    tester/config.cpp
    tester/main.cpp
    tester/tester.cpp)

set(HEADERS
    codec/codec.h
    common/defs.h
    common/utils.h
    huffman/huffman.h
    huffman/tree.h
    tester/config.h
    tester/tester.h)

add_executable(tester ${SOURCES})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Ofast")

if(GIT_BRANCH STREQUAL "master")
    message(STATUS "Release version")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
else()
    message(STATUS "Developer version")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -fno-omit-frame-pointer")
endif()

file(COPY tester.conf DESTINATION ./)
