cmake_minimum_required(VERSION 3.0)
project(tester)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(functions)
add_compile_flags(CMAKE_CXX_FLAGS)

include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR})

set(SOURCES
  main.cpp
  tester.cpp)

add_executable(tester ${SOURCES})
target_link_libraries(tester codec)
