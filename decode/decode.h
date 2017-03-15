//
// Created by vadim on 16.03.17.
//

#ifndef HUFFMAN_DECODE_H
#define HUFFMAN_DECODE_H

#include "decode_tree.h"

char tree_search(huffman_decode_tree *, unsigned char, short *, unsigned short, char);
void decode_file(const char *, char);

#endif //HUFFMAN_DECODE_H
