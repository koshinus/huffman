cmake_minimum_required(VERSION 3.3)
project(huffman)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99 -Wall")

set(SOURCE_FILES main.c huffman_tree.c huffman_tree.h support_func.c support_func.h)
add_executable(huffman ${SOURCE_FILES})