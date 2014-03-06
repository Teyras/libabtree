project(libabtree)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++11")

# add_library(libabtree STATIC abtree.h)

add_executable(test main.cpp)

# install(TARGETS libabtree RUNTIME DESTINATION bin)
