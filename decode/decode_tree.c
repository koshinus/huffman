//
// Created by vadim on 23.02.17.
//

#include "decode_tree.h"

huffman_decode_node make_decode_node(int16_t left, int16_t right, char symb)
{
    huffman_decode_node node;
    node.left = left;
    node.right = right;
    node.symbol = symb;
    return node;
}

huffman_decode_tree * make_decode_tree(FILE *f)
{
    char symbol;
    int16_t left, right;
    char number[2];
    number[0] = fgetc(f);
    number[1] = fgetc(f);
    uint16_t nodes_number = ((uint16_t *)number)[0];
    size_t size = offsetof(huffman_decode_tree, tree) + sizeof(huffman_decode_node) * nodes_number;
    huffman_decode_tree *hdt = (huffman_decode_tree *) malloc(size);
    hdt->nodes_number = nodes_number;
    for(size_t i = 0; i < nodes_number; i++)
    {
        symbol = fgetc(f);
        number[0] = fgetc(f);
        number[1] = fgetc(f);
        left = ((uint16_t *)number)[0];
        number[0] = fgetc(f);
        number[1] = fgetc(f);
        right = ((uint16_t *)number)[0];
        hdt->tree[i] = make_decode_node(left, right, symbol);
    }
    return hdt;
}
