//
// Created by vadim on 15.09.16.
//

#include "huffman_tree.h"

huffman_encode_node make_node(unsigned long freq, unsigned short cs, short l, short r, unsigned char leaf, unsigned char symb)
{
    huffman_encode_node node;
    node.frequency = freq;
    node.code_size = cs;
    node.left = l;
    node.right = r;
    node.free_and_is_leaf = leaf;
    node.code = "\0";
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
    unsigned short l_code_size, r_code_size;
    char *l_code, *r_code;
    if(l == -1) return;
    else
    {
		het->tree[l].code_size = het->tree[parent].code_size + (short)1;
        l_code_size = het->tree[l].code_size;
        l_code = (char *)malloc(sizeof(char)*(l_code_size+1));
        strcpy(l_code, het->tree[parent].code);
        l_code[l_code_size-1] = '0';
        l_code[l_code_size] = '\0';
        het->tree[l].code = l_code;
        get_huffman_codes_for_symbols(het, l);
        het->tree[r].code_size = het->tree[parent].code_size + (short)1;
        r_code_size = het->tree[r].code_size;
        r_code = (char *)malloc(sizeof(char)*(r_code_size+1));
        strcpy(r_code, het->tree[parent].code);
        r_code[r_code_size-1] = '1';
        r_code[r_code_size] = '\0';
        het->tree[r].code = r_code;
        get_huffman_codes_for_symbols(het, r);
    }
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
