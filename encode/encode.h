//
// Created by vadim on 16.03.17.
//

#ifndef HUFFMAN_ENCODE_H
#define HUFFMAN_ENCODE_H

#include "encode_tree.h"

short get_minimum(huffman_encode_tree *);
void get_huffman_codes_step(huffman_encode_tree *, short, short, char);
void get_huffman_codes_for_symbols(huffman_encode_tree *, short);
huffman_encode_node * get_huffman_node_by_symbol(huffman_encode_tree *, unsigned char);
void make_visualization(huffman_encode_tree *, const char *);
void write_tree_to_file(FILE *, huffman_encode_tree *);
void encode(FILE *, huffman_encode_tree *, const char *);
int encode_file(const char *, char, char);
void write_debug_info(huffman_encode_tree *, const char *, unsigned long long *, unsigned short);
void encode_step(FILE *, huffman_encode_node *, unsigned short *, char *, int, int, char);
void huffman_algorithm(huffman_encode_tree *, unsigned short);
void write_symbols_codes(FILE *, huffman_encode_tree *);

#endif //HUFFMAN_ENCODE_H
