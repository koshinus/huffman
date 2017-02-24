//
// Created by vadim on 15.09.16.
//

#include "encode_tree.h"

void encode_file(FILE *f)
{
    //Getting frequency table from text or binary file
    unsigned long long buffer[BUFFER_SIZE];
    get_frequency_table(f, buffer);

    unsigned short symbols_with_not_null_freq = count_symbols_with_not_null_frequency(buffer);

    //https://en.wikipedia.org/wiki/Huffman_coding
    //Algorithm steps:
    huffman_encode_tree *het = make_encode_tree(het, buffer, symbols_with_not_null_freq);			// 1
    huffman_algorithm(het, symbols_with_not_null_freq);                                     //2.1 - 2.3

    //Getting huffman codes for leafs
    get_huffman_codes_for_symbols(het, het->nodes_number-(short)1);

    //Visualize huffman tree
    make_visualization(het);

    //Encode file
    encode(f, het);

    free(het);
}

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

huffman_encode_tree * make_encode_tree(huffman_encode_tree *het, unsigned long long *frequency_table, unsigned short table_size)
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
        //unsigned short last_byte = (parent_bytes > 0)? parent_bytes - 1: 0;
        //unsigned short last_byte = get_limit(parent_bytes);
        unsigned short last_byte = LIMIT(parent_bytes);
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


void make_visualization(huffman_encode_tree *het)
{
    FILE *f;
    if ((f = fopen("/home/vadim/CLion/ClionProjects/huffman/tree_visualization.py", "w")) == NULL)
    {
        printf("Impossible to open file %s.\n", "tree_visualization.py\0");
        exit(1);
    }
    fprintf(f, "from graphviz import Digraph\n\n"
            "dot = Digraph('Huffman Tree')\ndot.attr('node', shape='box')\n");
    for (int i = 0; i < het->nodes_number; i++)
        fprintf(f, "dot.node(str(%d), label=str(%d) + '\\n_______\\n' + 'freq=' + str(%li))\n", i, i, het->tree[i].frequency);
    for (int i = het->nodes_number/2 + 1; i < het->nodes_number; i++)
    {
        fprintf(f, "dot.edge(str(%d), str(%d), label='0')\n", i, het->tree[i].left);
        fprintf(f, "dot.edge(str(%d), str(%d), label='1')\n", i, het->tree[i].right);
    }
    fprintf(f, "dot.render('huffman_tree', None, True)\n");
    fclose(f);
}


void encode(FILE *f, huffman_encode_tree *het)
{
    FILE *fout;
    if ( (fout = fopen("/home/vadim/CLion/ClionProjects/huffman/out.huf", "w")) == NULL )
    {
        printf("Impossible to open file %s.\n", "/home/vadim/CLion/ClionProjects/huffman/out.huf");
        exit(1);
    }
    else
    {
        //write_symbols_codes(fout, het);
        write_tree_to_file(fout, het);
        rewind(f);
        int c = EOF;
        char ch = 0;
        unsigned short bits_write_in_ch = 0;
        while( (c = fgetc(f)) != EOF )
        {
            huffman_encode_node *node = get_huffman_node_by_symbol(het, (unsigned char)c);
            //unsigned short limit = (node->code_size/LONG_LONG_SIZE > 0)? node->code_size/LONG_LONG_SIZE - 1: 0;
            //unsigned short limit = get_limit(node->code_size/LONG_LONG_SIZE);
            unsigned short limit = LIMIT(node->code_size/LONG_LONG_SIZE);
            for(int i = 0; i < limit; i++)
                for(int j = 0; j < LONG_LONG_SIZE; j++)
                    encode_step(fout, node, &bits_write_in_ch, &ch, i, j, 0);
            for (int i = 0; i < node->code_size % LONG_LONG_SIZE; i++)
                encode_step(fout, node, &bits_write_in_ch, &ch, limit, i, 1);
        }
        /*
        if(bits_write_in_ch != 0)
        {
            //fprintf(fout, "%c", ch << (BYTE_SIZE - bits_write_in_ch - 1));
            //fprintf(fout, "%d", bits_write_in_ch - 1);
            char s[bits_write_in_ch + 1]; s[bits_write_in_ch] = '\0';
            fprintf(fout, "%s", to_binary(s, (unsigned long long)ch, bits_write_in_ch));
            fprintf(fout, "%d", bits_write_in_ch);
        }
        */
        fclose(fout);
    }
}

/*
void write_symbols_codes(FILE *f, huffman_encode_tree *het)
{
    for(int i = 0; i < het->nodes_number/2 + 1; i++)
    {
        unsigned short elem_amount = het->tree[i].code_size / LONG_LONG_SIZE;
        char s1[LONG_LONG_SIZE + 1], s2[het->tree[i].code_size % LONG_LONG_SIZE + 1];
        s1[LONG_LONG_SIZE] = '\0';
        s2[het->tree[i].code_size % LONG_LONG_SIZE] = '\0';
        for (int j = 0; j < elem_amount; j++)
            fprintf(f, "%c%s", het->tree[i].symbol, to_binary(s1, het->tree[i].code[j], elem_amount));
        fprintf(f, "%c%s", het->tree[i].symbol,
                to_binary(s2, het->tree[i].code[elem_amount], het->tree[i].code_size % LONG_LONG_SIZE));
    }
    fprintf(f, "\n");
}
*/

void write_tree_to_file(FILE *f, huffman_encode_tree *het)
{
    fprintf(f, "%d", het->nodes_number);
    for(int i = 0; i < het->nodes_number; i++)
    {
        if(i < het->nodes_number/2 + 1)
            fprintf(f, "%c %hi %hi\n", het->tree[i].symbol, het->tree[i].left, het->tree[i].right);
        else fprintf(f, "- %hi %hi\n", het->tree[i].left, het->tree[i].right);
    }
}

void encode_step(FILE *f, huffman_encode_node *node, unsigned short *bits_write_in_ch, char *ch, int node_code_num, int count, char last)
{
    if(*bits_write_in_ch == BYTE_SIZE)
    {
        //char s[9]; s[8] = '\0';
        //fprintf(fout, "%s", to_binary(s, (unsigned long long)ch, BYTE_SIZE));
        fprintf(f, "%c", *ch);
        *ch = 0;
        *bits_write_in_ch = 0;
    }
    *ch <<= 1;
    *bits_write_in_ch++;
    if(last) *ch |= (node->code[node_code_num] >> (node->code_size % LONG_LONG_SIZE - count - 1)) & 1;
    else *ch |= (node->code[node_code_num] >> (LONG_LONG_SIZE - count - 1)) & 1;
}

void huffman_algorithm(huffman_encode_tree *het, unsigned short count)
{
    while(2*count - het->nodes_number > 1)			                        // 2
    {
        short left, right;
        left  = get_minimum(het);											// 2.1
        het->tree[ left].free_and_is_leaf |= 2;								// 2.1
        right = get_minimum(het);											// 2.1
        het->tree[right].free_and_is_leaf |= 2;								// 2.1
        huffman_encode_node node
                = make_encode_node(het->tree[ left].frequency +					// 2.2
                            het->tree[right].frequency ,	                // 2.2
                            0, left, right, 0, '-');						// 2.2
        het->tree[het->nodes_number] = node;					    		// 2.3
        het->nodes_number++;
    }
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
