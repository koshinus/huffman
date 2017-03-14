//
// Created by vadim on 15.09.16.
//

#include "encode_tree.h"

int encode_file(const char *filename, char text, char debug)
{
    FILE *fin;
    if(text) fin = fopen(filename, "r");
    else fin = fopen(filename, "rb");
    if(fin == NULL)
    {
        printf("Impossible to open file %s.\n", filename);
        exit(1);
    }
    //Getting frequency table from text or binary file
    unsigned long long buffer[BUFFER_SIZE];
    get_frequency_table(fin, buffer);

    unsigned short symbols_with_not_null_freq = count_symbols_with_not_null_frequency(buffer);

    //https://en.wikipedia.org/wiki/Huffman_coding
    //Algorithm steps:
    huffman_encode_tree *het = make_encode_tree(het, buffer, symbols_with_not_null_freq);			//1
    huffman_algorithm(het, symbols_with_not_null_freq);                                     //2.1 - 2.3

    //Getting huffman codes for leafs
    get_huffman_codes_for_symbols(het, het->nodes_number-(short)1);

    //Encode file
    encode(fin, het);

    if(debug)
        write_debug_info(het, filename, buffer, symbols_with_not_null_freq);

    free(het);
    fclose(fin);

    return 0;
}

void write_debug_info(huffman_encode_tree *het, const char *filename,
                      unsigned long long *buffer, unsigned short symbols_with_not_null_freq)
{
    char *dir_path = get_dir_path_from_full_path(dir_path, filename);
    char *log_path = (char *) malloc(strlen(dir_path) + strlen("info.log") + 1);
    FILE *log;
    if ((log = fopen(concat(log_path, dir_path, "info.log"), "w")) == NULL)
    {
        printf("Impossible to open file %s.\n", log_path);
        free(log_path);
        exit(1);
    }
    free(log_path);

    fprintf(log, "Frequency statistic for the file %s.\n", filename);
    for(int i = 0; i < BUFFER_SIZE; i++)
        fprintf(log, "%c - %lli\n", (char)i, buffer[i]);

    fprintf(log, "The number of characters encountered in the file at least once: %d", symbols_with_not_null_freq);

    for(int i = 0; i < het->nodes_number/2 + 1; i++)
    {
        unsigned short last_code_bytes = het->tree[i].code_size % LONG_LONG_SIZE;
        unsigned short limit = LIMIT(het->tree[i].code_size / LONG_LONG_SIZE);
        char *s = (char *)malloc((LONG_LONG_SIZE + 1)*sizeof(char)); s[LONG_LONG_SIZE] = '\0';
        fprintf(log, "%c (code length = %3hi) - ", het->tree[i].symbol, het->tree[i].code_size);
        for (int j = 0; j < limit; j++)
            fprintf(log, "%s", to_binary(s, het->tree[i].code[j], LONG_LONG_SIZE));
        free(s); s = (char *)malloc( (last_code_bytes + 1) * sizeof(char)); s[last_code_bytes] = '\0';
        fprintf(log, "%s\n", to_binary(s, het->tree[i].code[limit], last_code_bytes));
    }

    //Visualize huffman tree
    make_visualization(het);
}

void get_huffman_codes_for_symbols(huffman_encode_tree *het, short parent)
{
    short l = het->tree[parent].left, r = het->tree[parent].right;
    if(l == -1) return;
    else
    {
        get_huffman_codes_step(het, parent, l, 0);
        get_huffman_codes_step(het, parent, r, 1);
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
        unsigned short last_byte = LIMIT(parent_bytes);
        for(int i = 0; i < last_byte; i++)
            het->tree[child].code[i] = het->tree[parent].code[i];
        het->tree[child].code[last_byte] = het->tree[parent].code[last_byte] << 1;
        if(right) het->tree[child].code[last_byte]++;
    }
    get_huffman_codes_for_symbols(het, parent);
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

void write_tree_to_file(FILE *f, huffman_encode_tree *het)
{
    fprintf(f, "%d\n", het->nodes_number);
    for(int i = 0; i < het->nodes_number; i++)
    {
        if(i < het->nodes_number/2 + 1)
            fprintf(f, "%c %hi %hi\n", het->tree[i].symbol, het->tree[i].left, het->tree[i].right);
        else fprintf(f, "- %hi %hi\n", het->tree[i].left, het->tree[i].right);
    }
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
        write_tree_to_file(fout, het);
        rewind(f);
        int c = EOF;
        char ch = 0;
        unsigned short bits_write_in_ch = 0;
        while( (c = fgetc(f)) != EOF )
        {
            huffman_encode_node *node = get_huffman_node_by_symbol(het, (unsigned char)c);
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
            //fprintf(fout, "%d", bits_write_in_ch - 1);
            //fprintf(fout, "%c", ch << (BYTE_SIZE - bits_write_in_ch - 1));
            char s[bits_write_in_ch + 1]; s[bits_write_in_ch] = '\0';
            fprintf(fout, "%s", to_binary(s, (unsigned long long)ch, bits_write_in_ch));
            fprintf(fout, "%d", bits_write_in_ch);
        }
        */
        fclose(fout);
    }
}

void encode_step(FILE *f, huffman_encode_node *node, unsigned short *bits_write_in_ch,
                 char *ch, int node_code_num, int count, char last)
{
    if(*bits_write_in_ch == BYTE_SIZE)
    {
        //char s[9]; s[8] = '\0';
        //fprintf(fout, "%s", to_binary(s, (unsigned long long)ch, BYTE_SIZE));
        fprintf(f, "%c", *ch);
        *ch = 0;
        *bits_write_in_ch = 0;
    }
    (*ch) <<= 1;
    (*bits_write_in_ch)++;
    if(last) (*ch) |= (node->code[node_code_num] >> (node->code_size % LONG_LONG_SIZE - count - 1)) & 1;
    else (*ch) |= (node->code[node_code_num] >> (LONG_LONG_SIZE - count - 1)) & 1;
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
                = make_encode_node(het->tree[ left].frequency +				// 2.2
                            het->tree[right].frequency,	                    // 2.2
                            0, left, right, 0, '-');						// 2.2
        het->tree[het->nodes_number] = node;					    		// 2.3
        het->nodes_number++;
    }
}