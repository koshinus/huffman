//
// Created by vadim on 16.03.17.
//

#include "encode.h"

void write_debug_info(huffman_encode_tree *het, const char *fpath,
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

    fprintf(log, "Frequency statistic for the file %s.\n", fpath);
    for(int i = 0; i < BUFFER_SIZE; i++)
        fprintf(log, "%c - %lli\n", (char)i, buffer[i]);

    fprintf(log, "The number of characters encountered in the file at least once: %d", symbols_with_not_null_freq);

    write_symbols_codes(log, het);

    //Visualize huffman tree
    make_visualization(het, fpath);
}

void write_symbols_codes(FILE *log, huffman_encode_tree *het)
{
    for(int i = 0; i < het->nodes_number/2 + 1; i++)
    {
        unsigned short last_code_bytes = het->tree[i].code_size % LL_SIZE;
        unsigned short limit = LIMIT(het->tree[i].code_size / LL_SIZE);
        char *s = (char *)malloc((LL_SIZE + 1)*sizeof(char)); s[LL_SIZE] = '\0';
        fprintf(log, "%c (code length = %3hi) - ", het->tree[i].symbol, het->tree[i].code_size);
        for (int j = 0; j < limit; j++)
            fprintf(log, "%s", to_binary(s, het->tree[i].code[j], LL_SIZE));
        free(s);
        s = (char *)malloc( (last_code_bytes + 1) * sizeof(char)); s[last_code_bytes] = '\0';
        fprintf(log, "%s\n", to_binary(s, het->tree[i].code[limit], last_code_bytes));
        free(s);
    }
}

void make_visualization(huffman_encode_tree *het, const char *fpath)
{
    char *dir_path = get_dir_path_from_full_path(fpath);
    char *vis_path = concat(dir_path, "tree_visualization.py");
    FILE *f;
    if ((f = fopen(vis_path, "w")) == NULL)
    {
        printf("Impossible to open file %s.\n", vis_path);
        free(vis_path);
        exit(1);
    }
    free(vis_path);

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