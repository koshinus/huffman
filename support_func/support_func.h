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
/*
 * IMPORTANT! SET THE OUT_BUFFER_SIZE CAREFULLY:
 * THERE IS NO CHECKS FOR RANGE WHEN WRITING
 * ENCODING TREE TO OUTPUT BUFFER
*/
#define        IN_BUFFER_SIZE (4*1024)
#define       OUT_BUFFER_SIZE (4*1024)

// Number of handled bytes in decode/encode buffers
uint64_t  in_bytes_count;
uint64_t out_bytes_count;
uint64_t some_var;

int8_t  in_buffer[ IN_BUFFER_SIZE];
int8_t out_buffer[OUT_BUFFER_SIZE];

#define LIMIT(X) ((X > 0)? (X-1): (0))

char * to_binary(uint64_t, uint16_t);
void get_frequency_table(FILE *, uint64_t *);
uint16_t count_symbols_with_not_null_frequency(uint64_t *);
char * get_dir_path_from_full_path(const char *);
char * concat(const char *, const char *);
uint64_t position_in_buffer(uint16_t nodes_number, uint8_t decode_node_size);

#endif //HUFFMAN_DEBUG_H
