//
// Created by vadim on 15.09.16.
//

#include "encode_tree.h"

huffman_encode_node make_encode_node(unsigned long freq, unsigned short cs,
                                     short l, short r, unsigned char leaf, unsigned char symb)
{
    huffman_encode_node node;
    node.code = (unsigned long long *)calloc(1, sizeof(unsigned long long));
    node.frequency = freq;
    node.code_size = cs;
    node.left = l;
    node.right = r;
    node.free_and_is_leaf = leaf;
    node.symbol = symb;
    return node;
}

huffman_encode_tree * make_encode_tree(unsigned long long *frequency_table, unsigned short table_size)
{
    size_t size = offsetof(huffman_encode_tree, tree) + sizeof(huffman_encode_node) * (2 * table_size - 1);
    huffman_encode_tree *het = (huffman_encode_tree *) malloc(size);
    unsigned short tree_count = 0;
    for(int i = 0; i < BUFFER_SIZE; i++)
    {
        if(frequency_table[i] != 0)
        {
            het->tree[tree_count] = make_encode_node(frequency_table[i], 0, -1, -1, 1, (unsigned char)i);
            tree_count++;
        }
    }
    het->nodes_number = table_size;
    return het;
}