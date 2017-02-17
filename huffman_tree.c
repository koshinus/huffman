//
// Created by vadim on 15.09.16.
//

#include "huffman_tree.h"

huffman_encode_node make_node(unsigned long freq, unsigned short cs,
                              short l, short r, unsigned char leaf, unsigned char symb)
{
    huffman_encode_node node;
    /*
    {
        unsigned long long new_code[4] = {0};
        memcpy(node.code, new_code, sizeof(new_code));
    }
    */
    node.code = (unsigned long long *)calloc(1, sizeof(unsigned long long));
    node.frequency = freq;
    node.code_size = cs;
    node.left = l;
    node.right = r;
    node.free_and_is_leaf = leaf;
	node.symbol = symb;
    return node;
}

huffman_encode_tree * make_tree(huffman_encode_tree *het, unsigned long long *frequency_table, unsigned short table_size)
{
    size_t size = offsetof(huffman_encode_tree, tree) + sizeof(huffman_encode_node) * (2 * table_size - 1);
    het = (huffman_encode_tree *) malloc(size);
    unsigned short tree_count = 0;
    for(int i = 0; i < BUFFER_SIZE; i++)
    {
        if(frequency_table[i] != 0)
        {
            het->tree[tree_count] = make_node(frequency_table[i], 0, -1, -1, 1, (unsigned char)i);
            tree_count++;
        }
    }
    het->nodes_number = table_size;
    return het;
}

long long cmp(huffman_encode_node *l, huffman_encode_node *r)
{
    return (long long)(l->frequency) - (long long)(r->frequency);
}

void get_huffman_codes_for_symbols(huffman_encode_tree *het, short parent)
{
    short l = het->tree[parent].left, r = het->tree[parent].right;
    if(l == -1) return;
    else
    {
        get_huffman_codes_step(het, parent, l, 0);
        get_huffman_codes_for_symbols(het, l);
        get_huffman_codes_step(het, parent, r, 1);
        get_huffman_codes_for_symbols(het, r);
    }
}

void get_huffman_codes_step(huffman_encode_tree *het, short parent, short child, char right)
{
    /*TODO: document all byte operations!!!!!
     * */
    het->tree[child].code_size = het->tree[parent].code_size + (short)1;
    unsigned short parent_bytes = het->tree[parent].code_size/LONG_LONG_SIZE;
    unsigned short  child_bytes = het->tree[ child].code_size/LONG_LONG_SIZE;
    if(child_bytes > parent_bytes)
    {
        het->tree[child].code = (unsigned long long *)calloc(child_bytes, sizeof(unsigned long long));
        for(int i = 0; i < parent_bytes; i++)
            het->tree[child].code[i] = het->tree[parent].code[i];
        if(right) het->tree[child].code[parent_bytes] = 1;
    }
    else
    {
        unsigned short last_byte = (parent_bytes > 0)? parent_bytes - 1: 0;
        for(int i = 0; i < last_byte; i++)
            het->tree[child].code[i] = het->tree[parent].code[i];
        het->tree[child].code[last_byte] = het->tree[parent].code[last_byte] << 1;
        if(right) het->tree[child].code[last_byte]++;
    }
}

huffman_encode_node * get_huffman_node_by_symbol(huffman_encode_tree *het, unsigned char symbol)
{
    for(int i = 0; i < het->nodes_number/2 + 1; i++)
        if(het->tree[i].symbol == symbol)
            return &het->tree[i];
}

short get_minimum(huffman_encode_tree *het)
{
    short i = 0, min = 0;
    for(i; i < het->nodes_number; i++)
        if ((het->tree[i].free_and_is_leaf & 2) != 2)
        {
            min = i;
            break;
        }
    for(i; i < het->nodes_number; i++)
    {
        if ((het->tree[i].free_and_is_leaf & 2) != 2 &&
                het->tree[i].frequency < het->tree[min].frequency)
            min = i;
    }
    return min;
}

/*
het->tree[l].code_size = p_code_size + (short)1;
for(int i = 0; i < byte_number;i++)
    het->tree[l].code[i] = het->tree[parent].code[i];
if(byte_number <= 3)
    het->tree[l].code[byte_number] = het->tree[parent].code[byte_number];
    //het->tree[r].code[byte_number] = het->tree[parent].code[byte_number] << 1;
//het->tree[l].code[p_code_size/BYTE_SIZE+1] += 0 << (BYTE_SIZE - (p_code_size%BYTE_SIZE));
//het->tree[l].code[(p_code_size + 1)/BYTE_SIZE] = het->tree[parent].code[p_code_size/BYTE_SIZE] << 1;
get_huffman_codes_for_symbols(het, l);
het->tree[r].code_size = het->tree[parent].code_size + (short)1;
for(int i = 0; i < byte_number;i++)
    het->tree[r].code[i] = het->tree[parent].code[i];
if(byte_number <= 3)
{
    het->tree[r].code[byte_number] = het->tree[parent].code[byte_number];
    //het->tree[r].code[byte_number] = (het->tree[parent].code[byte_number] << 1) + 1;
    het->tree[r].code[byte_number] |= 1 << (LONG_LONG_SIZE - (p_code_size % LONG_LONG_SIZE));
}
get_huffman_codes_for_symbols(het, r);
*/

/*
void get_minimums(huffman_encode_tree *het, short *left, short *right)
{
    short i = 0;
    for(i; i < het->nodes_number; i++)
        if ((het->tree[i].free_and_is_leaf & 2) != 2)
        {
            *left = *right = i;
            break;
        }
    int first_iteration = 0;
    if(i == 139)
    {
        printf("wow");
        printf("wow\n");
    }
    for(i; i < het->nodes_number; i++)
    {
        if(het->tree[i].frequency < het->tree[*left].frequency &&
           (het->tree[i].free_and_is_leaf & 2) != 2)
        {
            if(first_iteration == 0)
            {
                *left = i;
                first_iteration = 1;
            }
            else
            {
                *right = *left;
                *left = i;
            }
        }
    }
    //if(right == 0) printf("%d, %d\n", (int)het->tree[*right].free_and_is_leaf, het->nodes_number);
}

short free_nodes_number(huffman_encode_tree *het)
{
    short num = 0;
    for(int i = 0; i < het->nodes_number; i++)
        if((het->tree[i].free_and_is_leaf & 2) == 0) num++;
    return num;
}

void free_tree(huffman_encode_tree *het)
{
    free(het->tree);
}

unsigned short get_nodes_number(huffman_encode_tree *het)
{
    return het->nodes_number;
}

void set_nodes_number(huffman_encode_tree *het, unsigned short n)
{
    het->nodes_number = n;
}

huffman_encode_node * get_tree_node(huffman_encode_tree *het, unsigned short position)
{
    return &(het->tree[position]);
}
*/
