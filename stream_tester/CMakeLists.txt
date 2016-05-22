cmake_minimum_required(VERSION 3.0)
project(stream_tester)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(functions)
add_compile_flags(CMAKE_CXX_FLAGS)

include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR})

set(SOURCES
  main.cpp
  stream_tester.cpp)

add_executable(stream_tester ${SOURCES})
target_link_libraries(stream_tester codec)
