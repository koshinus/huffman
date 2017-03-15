//
// Created by vadim on 15.09.16.
//

#ifndef HUFFMAN_HUFFMAN_TREE_H
#define HUFFMAN_HUFFMAN_TREE_H

#include "../support_func/support_func.h"

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
huffman_encode_tree * make_encode_tree(unsigned long long *, unsigned short);


#endif //HUFFMAN_HUFFMAN_TREE_H
