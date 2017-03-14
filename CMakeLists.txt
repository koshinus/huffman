cmake_minimum_required(VERSION 3.3)
project(huffman)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99 -Wall")

set(SOURCE_FILES main.c encode_tree.c encode_tree.h decode_tree.c decode_tree.h support_func.c support_func.h)
#set(SOURCE_FILES test.c)
add_executable(huffman ${SOURCE_FILES})