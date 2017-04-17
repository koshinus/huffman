//
// Created by vadim on 16.03.17.
//

#ifndef HUFFMAN_ENCODE_H
#define HUFFMAN_ENCODE_H

#include "encode_tree.h"

int16_t get_minimum(huffman_encode_tree *);
void get_huffman_codes_step(huffman_encode_tree *, int16_t, int16_t, int8_t);
void get_huffman_codes_for_symbols(huffman_encode_tree *, int16_t);
huffman_encode_node * get_huffman_node_by_symbol(huffman_encode_tree *, unsigned char);
void make_visualization(FILE *, huffman_encode_tree *);
void write_tree_to_file(FILE *, huffman_encode_tree *);
void encode(FILE *, huffman_encode_tree *, const char *);
int32_t encode_file(const char *, int8_t, int8_t);
void write_debug_info(FILE *, huffman_encode_tree *, const char *, uint64_t *, uint16_t);
void encode_step(FILE *, huffman_encode_node *, uint16_t *, char *, int32_t, int32_t, int8_t);
void huffman_algorithm(huffman_encode_tree *, uint16_t);
void write_symbols_codes(FILE *, huffman_encode_tree *);
void info_encode(FILE *, FILE *, huffman_encode_tree *);
void info_encode_step(FILE *, huffman_encode_node *, uint16_t *, char *, int32_t, int32_t, int8_t);

#endif //HUFFMAN_ENCODE_H
