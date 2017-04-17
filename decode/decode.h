//
// Created by vadim on 16.03.17.
//

#ifndef HUFFMAN_DECODE_H
#define HUFFMAN_DECODE_H

#include "decode_tree.h"

int16_t tree_search(huffman_decode_tree *, unsigned char *, int16_t, int16_t, int8_t *);
void decode_file(const char *, int8_t);
void decode(huffman_decode_tree *, FILE *, FILE *);

#endif //HUFFMAN_DECODE_H
