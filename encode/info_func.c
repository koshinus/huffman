//
// Created by vadim on 16.03.17.
//

#include "encode.h"

void write_debug_info(FILE *f, huffman_encode_tree *het, const char *fpath,
                      unsigned long long *buffer, unsigned short symbols_with_not_null_freq)
{

    char *dir_path = get_dir_path_from_full_path(fpath);

    char *log_path = concat(dir_path, "info.log");
    FILE *log;
    if ((log = fopen(log_path, "w")) == NULL)
    {
        printf("Impossible to open file %s.\n", log_path);
        free(dir_path); free(log_path);
        exit(1);
    }
    free(dir_path); free(log_path);

    char *vis_path = concat(dir_path, "tree_visualization.py");
    FILE *vis;
    if ((vis = fopen(vis_path, "w")) == NULL)
    {
        printf("Impossible to open file %s.\n", vis_path);
        //free(vis_path);
        exit(1);
    }
    //free(vis_path);

    //Writing frequency statistic to log file
    fprintf(log, "Frequency statistic for the file %s.\n", fpath);
    for(int i = 0; i < BUFFER_SIZE; i++)
        fprintf(log, "%c - %lli\n", (char)i, buffer[i]);

    fprintf(log, "The number of characters encountered in the file at least once: %d", symbols_with_not_null_freq);

    write_symbols_codes(log, het);

    //Visualize huffman tree
    make_visualization(vis, het);
    char *sys_arg = concat("python ", vis_path);
    system(sys_arg);
    free(vis_path); free(sys_arg);

    //Write encoded file to log file in binary view
    info_encode(f, log, het);
}

void write_symbols_codes(FILE *log, huffman_encode_tree *het)
{
    for(int i = 0; i < het->nodes_number/2 + 1; i++)
    {
        unsigned short last_code_bytes = het->tree[i].code_size % LL_SIZE;
        unsigned short limit = LIMIT(het->tree[i].code_size / LL_SIZE);
        //char *s = (char *)malloc((LL_SIZE + 1)*sizeof(char)); s[LL_SIZE] = '\0';
        char *s;
        fprintf(log, "%c (code length = %3hi) - ", het->tree[i].symbol, het->tree[i].code_size);
        for (int j = 0; j < limit; j++)
            fprintf(log, "%s", (s = to_binary(het->tree[i].code[j], LL_SIZE)));
        if(limit > 0) free(s);
        //s = (char *)malloc((last_code_bytes + 1) * sizeof(char)); s[last_code_bytes] = '\0';
        fprintf(log, "%s\n", (s = to_binary(het->tree[i].code[limit], last_code_bytes)));
        free(s);
    }
}

void make_visualization(FILE *f, huffman_encode_tree *het)
{
    fprintf(f, "from graphviz import Digraph\n\n"
            "dot = Digraph('Huffman Tree')\ndot.attr('node', shape='box')\n");
    for (int i = 0; i < het->nodes_number; i++)
        fprintf(f, "dot.node(str(%d), label=str(%d) + '\\n_______\\n' + 'freq=' + str(%li))\n",
                i, i, het->tree[i].frequency);
    for (int i = het->nodes_number/2 + 1; i < het->nodes_number; i++)
    {
        fprintf(f, "dot.edge(str(%d), str(%d), label='0')\n", i, het->tree[i].left);
        fprintf(f, "dot.edge(str(%d), str(%d), label='1')\n", i, het->tree[i].right);
    }
    fprintf(f, "dot.render('huffman_tree', None, True)\n");
    fclose(f);
}

void info_encode(FILE *f, FILE *log, huffman_encode_tree *het)
{
    write_tree_to_file(log, het);
    rewind(f);
    int c = EOF;
    char ch = 0;
    unsigned short bits_write_in_ch = 0;
    while( (c = fgetc(f)) != EOF )
    {
        huffman_encode_node *node = get_huffman_node_by_symbol(het, (unsigned char)c);
        unsigned short limit = LIMIT(node->code_size/LL_SIZE);
        for(int i = 0; i < limit; i++)
            for(int j = 0; j < LL_SIZE; j++)
                info_encode_step(log, node, &bits_write_in_ch, &ch, i, j, 0);
        for (int i = 0; i < node->code_size % LL_SIZE; i++)
            info_encode_step(log, node, &bits_write_in_ch, &ch, limit, i, 1);
    }
    if(bits_write_in_ch != 0)
    {
        char *s;
        fprintf(log, "%d", bits_write_in_ch);
        fprintf(log, "%s", (s = to_binary((unsigned long long)ch, bits_write_in_ch)));
        free(s);
    }
}

void info_encode_step(FILE *f, huffman_encode_node *node, unsigned short *bits_write_in_ch,
                 char *ch, int node_code_num, int count, char last)
{
    if(*bits_write_in_ch == BYTE_SIZE)
    {
        char *s = to_binary((unsigned long long)(*ch), BYTE_SIZE);
        fprintf(f, "%s", s);
        free(s);
        *ch = 0;
        *bits_write_in_ch = 0;
    }
    (*ch) <<= 1;
    (*bits_write_in_ch)++;
    if(last) (*ch) |= (node->code[node_code_num] >> (node->code_size % LL_SIZE - count - 1)) & 1;
    else (*ch) |= (node->code[node_code_num] >> (LL_SIZE - count - 1)) & 1;
}
