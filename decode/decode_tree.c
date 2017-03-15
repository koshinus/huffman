//
// Created by vadim on 23.02.17.
//

#include "decode_tree.h"

huffman_decode_node make_decode_node(short left, short right, char symb)
{
    huffman_decode_node node;
    node.left = left;
    node.right = right;
    node.symbol = symb;
    return node;
}

huffman_decode_tree * make_decode_tree(FILE *f)
{
    unsigned short nodes_number;
    char c;
    fscanf(f, "%hi%c", &nodes_number, &c);
    size_t size = offsetof(huffman_decode_tree, tree) + sizeof(huffman_decode_node) * nodes_number;
    huffman_decode_tree *hdt = (huffman_decode_tree *)malloc(size);
    for(size_t i = 0; i < nodes_number; i++)
    {
        char symbol;
        short left, right;
        fscanf(f, "%c%hi%hi%c", &symbol, &left, &right, &c);
        hdt->tree[i] = make_decode_node(left, right, symbol);
    }
    return hdt;
}

