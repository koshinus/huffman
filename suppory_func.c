//
// Created by vadim on 08.02.17.
//

#include "suppory_func.h"
#include "huffman_tree.h"

void make_visualization(huffman_encode_tree *het)
{
    FILE *f;
    if ((f = fopen("../tree_visualization.py", "w")) == NULL)
    {
        printf("Impossible to open file %s.\n", "tree_visualization.py");
        exit(1);
    }
    fprintf(f, "from graphviz import Digraph\n\n"
            "dot = Digraph('Huffman Tree')\ndot.attr('node', shape='box')\n");
    for (int i = 0; i < BUFFER_SIZE * 2 - 1; i++)
        fprintf(f, "dot.node(str(%d), label=str(%d) + '\\n_______\\n' + 'freq=' + str(%li))\n", i, i, het->tree[i].frequency);
    for (int i = BUFFER_SIZE; i < BUFFER_SIZE * 2 - 1; i++)
    {
        fprintf(f, "dot.edge(str(%d), str(%d))\n", i, het->tree[i].left);
        fprintf(f, "dot.edge(str(%d), str(%d))\n", i, het->tree[i].right);
    }
    fprintf(f, "dot.render('huffman_tree', None, True)\n");
    fclose(f);
}