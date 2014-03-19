project(libabtree)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

# add_library(libabtree STATIC abtree.h)

add_executable(test src/test.cpp)

add_executable(benchmark src/benchmark.cpp)

# install(TARGETS libabtree RUNTIME DESTINATION bin)

add_subdirectory(src)