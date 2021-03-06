//
// Created by vadim on 08.02.17.
//

#ifndef HUFFMAN_DEBUG_H
#define HUFFMAN_DEBUG_H

#include<stdio.h>
#include<stddef.h>
#include<string.h>
#include<stdlib.h>
#include<inttypes.h>

#define         BYTE_SIZE 8
#define           LL_SIZE 64
#define       BUFFER_SIZE 256
#define       IN_BUFFER_SIZE 4*1024
#define       OUT_BUFFER_SIZE 4*1024

// Number of handled bytes in decode/encode buffers
uint16_t in_bytes_count;
uint16_t out_bytes_count;

int8_t in_buffer[IN_BUFFER_SIZE];
int8_t out_buffer[OUT_BUFFER_SIZE];

/*
#define         TEXT_READ 11
#define       BINARY_READ 12
#define        TEXT_WRITE 21
#define      BINARY_WRITE 22
#define   TEXT_READ_WRITE 13
#define BINARY_READ_WRITE 14
#define      TEXT_REWRITE 23
#define    BINARY_REWRITE 24
*/

#define LIMIT(X) ((X > 0)? (X-1): (0))

/*
char file_info[2][4] = {{"r", "rb", "r+", "rb+"},
                        {"w", "wb", "w+", "wb+"}};
*/

char * to_binary(uint64_t, uint16_t);
void get_frequency_table(FILE *, uint64_t *);
uint16_t count_symbols_with_not_null_frequency(uint64_t *);
char * get_dir_path_from_full_path(const char *);
char * concat(const char *, const char *);

#endif //HUFFMAN_DEBUG_H
