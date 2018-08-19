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
    int16_t nodes_number, nodes_handled = 0, left, right;
    char symbol;
    int8_t first_read = 1, decode_node_size = sizeof(huffman_decode_node);
    huffman_decode_tree *hdt;
    uint64_t i = 0;
    while (1)
    {
        in_bytes_count = fread(in_buffer + i, sizeof(int8_t), IN_BUFFER_SIZE - i, f);
        i = 0;
        if(first_read)
        {
            nodes_number = ((uint16_t *)in_buffer)[0];
            i = sizeof(nodes_number);
            size_t size = offsetof(huffman_decode_tree, tree) + sizeof(huffman_decode_node) * nodes_number;
            hdt = (huffman_decode_tree *) malloc(size);
            hdt->nodes_number = nodes_number;
            first_read = 0;
        }
        while(1)
        {
            symbol = in_buffer[i];
            left = ((uint16_t *)(in_buffer + i + 1))[0];
            right = ((uint16_t *)(in_buffer + i + 1))[1];
            i += decode_node_size;
            hdt->tree[nodes_handled++] = make_decode_node(left, right, symbol);
            if(nodes_handled == nodes_number)
            {
                some_var = i;
                return hdt;
            }
            if(in_bytes_count - i < decode_node_size)
            {
                for(uint64_t j = 0; j < in_bytes_count - i; j++)
                    in_buffer[j] = in_buffer[i + j];
                i = in_bytes_count - i;
                break;
            }
        }
    }
}
