cmake_minimum_required(VERSION 3.12)
project(huffman)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -D_GLIBCXX_DEBUG")

add_executable(util utility.cpp)
add_executable(testing gtest/gtest.h gtest/gtest-all.cc gtest/gtest_main.cc test/testing_huffman.cpp)