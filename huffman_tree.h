//
// Created by vadim on 15.09.16.
//

#ifndef HUFFMAN_HUFFMAN_TREE_H
#define HUFFMAN_HUFFMAN_TREE_H

#include<stdio.h>
#include<stddef.h>
#include<string.h>
#include<stdlib.h>
#include "support_func.h"

#define      BYTE_SIZE 8
#define LONG_LONG_SIZE 64
#define    BUFFER_SIZE 256

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

long long cmp(huffman_encode_node *, huffman_encode_node *);
huffman_encode_node make_node(unsigned long, unsigned short, short, short, unsigned char, unsigned char);
huffman_encode_tree * make_tree(huffman_encode_tree *, unsigned long long *, unsigned short);
short get_minimum(huffman_encode_tree *);
void get_huffman_codes_step(huffman_encode_tree *, short, short, char);
void get_huffman_codes_for_symbols(huffman_encode_tree *, short);
huffman_encode_node * get_huffman_node_by_symbol(huffman_encode_tree *, unsigned char);
void make_visualization(huffman_encode_tree *);
void write_symbols_codes(FILE *, huffman_encode_tree *);
void encode_file(FILE *, huffman_encode_tree *);
void encode_step(FILE *, unsigned short, char);
void huffman_algorithm(huffman_encode_tree *, unsigned short);

#endif //HUFFMAN_HUFFMAN_TREE_H
