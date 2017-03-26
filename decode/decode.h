//
// Created by vadim on 16.03.17.
//

#ifndef HUFFMAN_DECODE_H
#define HUFFMAN_DECODE_H

#include "decode_tree.h"

short tree_search(huffman_decode_tree *, unsigned char *, short, short, char *);
void decode_file(const char *, char);
void decode(huffman_decode_tree *, FILE *, FILE *);

#endif //HUFFMAN_DECODE_H
