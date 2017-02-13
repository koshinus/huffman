//
// Created by vadim on 08.02.17.
//

#include "suppory_func.h"
//#include "huffman_tree.h"

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

char * to_binary(char *s, unsigned long long code)
{
    for(int i = 0; i < 64; i++)
        s[63-i] = (char)((code >> i) & 1) + 48;
    return s;
}