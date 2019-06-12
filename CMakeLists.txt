cmake_minimum_required(VERSION 3.12)
project(huffman)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -D_GLIBCXX_DEBUG")

add_library(huffman_lib STATIC huffman.h huffman.cpp)

add_executable(util utility.cpp)
target_link_libraries(util huffman_lib)

add_executable(test gtest/gtest.h gtest/gtest-all.cc gtest/gtest_main.cc test/testing_huffman.cpp)
target_link_libraries(test huffman_lib)