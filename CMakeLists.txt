cmake_minimum_required(VERSION 3.4)
project(tester)

execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    -Wall -Wextra -Weverything -std=c++14 -Ofast -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} \
    -g -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Werror")

if(GIT_BRANCH STREQUAL "master")
    message(STATUS "Release version")
    set(CMAKE_BUILD_TYPE Release)
else()
    message(STATUS "Developer version")
    set(CMAKE_BUILD_TYPE Debug)
endif()

file(COPY tester/tester.conf DESTINATION ./)
