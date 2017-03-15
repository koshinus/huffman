//
// Created by vadim on 23.02.17.
//

#ifndef HUFFMAN_DECODE_TREE_H
#define HUFFMAN_DECODE_TREE_H

#include "../support_func/support_func.h"

typedef struct huffman_decode_node_
{
    short left, right;
    char symbol;
} huffman_decode_node;

typedef struct huffman_decode_tree_
{
    unsigned short nodes_number;
    huffman_decode_node tree[];
} huffman_decode_tree;

huffman_decode_node make_decode_node(short, short, char);
huffman_decode_tree * make_decode_tree(FILE *);

#endif //HUFFMAN_DECODE_TREE_H