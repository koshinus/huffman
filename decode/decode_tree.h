//
// Created by vadim on 23.02.17.
//

#ifndef HUFFMAN_DECODE_TREE_H
#define HUFFMAN_DECODE_TREE_H

#include "../support_func/support_func.h"

typedef struct huffman_decode_node_
{
    int16_t left, right;
    char symbol;
} __attribute__((packed, aligned(1))) huffman_decode_node;

typedef struct huffman_decode_tree_
{
    uint16_t nodes_number;
    huffman_decode_node tree[];
} huffman_decode_tree;

huffman_decode_node make_decode_node(int16_t, int16_t, char);
huffman_decode_tree * make_decode_tree(FILE *);

#endif //HUFFMAN_DECODE_TREE_H
