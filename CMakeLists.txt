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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

if(GIT_BRANCH STREQUAL "master")
    message(STATUS "Release version")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
    add_executable(tester ${SOURCES})
else()
    message(STATUS "Developer version")

    add_executable(tester_run ${SOURCES})
    set_target_properties(tester_run PROPERTIES COMPILE_FLAGS "-Ofast \
        -DTESTER_RUN")

    add_executable(tester_dbg ${SOURCES})
    set_target_properties(tester_dbg PROPERTIES LINK_FLAGS "-pg")
    set_target_properties(tester_dbg PROPERTIES COMPILE_FLAGS "-O0 -ggdb")
endif()

file(COPY tester.conf DESTINATION ./)
