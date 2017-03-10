//
// Created by vadim on 15.09.16.
//

#ifndef HUFFMAN_HUFFMAN_TREE_H
#define HUFFMAN_HUFFMAN_TREE_H

#include "support_func.h"

typedef struct huffman_encode_node_
{
    unsigned long long *code;           //Huffman code for symbol
    unsigned long frequency;		    //Summary frequency of node
	unsigned short code_size;		    //Number of useful bits in "code" variable
    short left, right;			        //Left and right parents of node
    unsigned char free_and_is_leaf,	    //Variable in binary format is equal:
			                            //00000abc c = 1 if node is a leaf, else c = 0
			                            //00000abc b = 1 if node have been used as a parent before, else b = 0
			                            //00000abc a = 1 if node have been visited, else a = 0
    symbol;							    //Symbol in node if node is leaf, else - '-'
} huffman_encode_node;

typedef struct huffman_encode_tree_
{
    unsigned short nodes_number;
    huffman_encode_node tree[];
} huffman_encode_tree;

huffman_encode_node make_encode_node(unsigned long, unsigned short, short, short, unsigned char, unsigned char);
huffman_encode_tree * make_encode_tree(huffman_encode_tree *, unsigned long long *, unsigned short);
short get_minimum(huffman_encode_tree *);
void get_huffman_codes_step(huffman_encode_tree *, short, short, char);
void get_huffman_codes_for_symbols(huffman_encode_tree *, short);
huffman_encode_node * get_huffman_node_by_symbol(huffman_encode_tree *, unsigned char);
void make_visualization(huffman_encode_tree *);
//void write_symbols_codes(FILE *, huffman_encode_tree *);
void write_tree_to_file(FILE *, huffman_encode_tree *);
void encode(FILE *, huffman_encode_tree *);
void encode_file(FILE *);
void encode_step(FILE *, huffman_encode_node *, unsigned short *, char *, int, int, char);
void huffman_algorithm(huffman_encode_tree *, unsigned short);


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

huffman_encode_tree * make_encode_tree(huffman_encode_tree *het,
                                       unsigned long long *frequency_table, unsigned short table_size)
{
    size_t size = offsetof(huffman_encode_tree, tree) + sizeof(huffman_encode_node) * (2 * table_size - 1);
    het = (huffman_encode_tree *) malloc(size);
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

#endif //HUFFMAN_HUFFMAN_TREE_H
