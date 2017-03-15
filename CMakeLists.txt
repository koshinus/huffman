cmake_minimum_required(VERSION 3.3)
project(huffman)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99 -Wall")

set(SOURCE_FILES main.c
        encode/encode_tree.c encode/encode_tree.h
        encode/general_encode_steps.c encode/info_func.c encode/support_encode_steps.c encode/encode.h

        decode/decode_tree.c decode/decode_tree.h
        decode/general_decode_steps.c decode/support_decode_steps.c decode/decode.h

        support_func/support_func.c support_func/support_func.h)
#set(SOURCE_FILES test.c)

add_executable(huffman ${SOURCE_FILES})