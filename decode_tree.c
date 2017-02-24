//
// Created by vadim on 23.02.17.
//

#include "decode_tree.h"

huffman_decode_node make_decode_node(short left, short right, char symb)
{
    huffman_decode_node node;
    node.left = left;
    node.right = right;
    node.symbol = symb;
    return node;
}

huffman_decode_tree * make_decode_tree(FILE *f, huffman_decode_tree *hdt)
{
    unsigned short nodes_number;
    fscanf(f, "%hi", &nodes_number);
    size_t size = offsetof(huffman_decode_tree, tree) + sizeof(huffman_decode_node) * nodes_number;
    hdt = (huffman_decode_tree *)malloc(size);
    for(size_t i = 0; i < nodes_number; i++)
    {
        char symbol, c;
        short left, right;
        fscanf(f, "%c%hi%hi%c", &symbol, &left, &right, &c);
        hdt->tree[i] = make_decode_node(left, right, symbol);
    }
    return hdt;
}

char tree_search(huffman_decode_tree *hdt, unsigned char code, unsigned short *pos, unsigned short root, char readed_bits)
{
    short l = hdt->tree[*pos].left, r = hdt->tree[*pos].right;
    if(l == -1)
    {
        char c = hdt->tree[*pos].symbol;
        *pos = root;
        return c;
    }
    else if(readed_bits == 8) return '-';
    else
    {
        readed_bits++;
        if(code << 7 == 0) return tree_search(hdt, code << 1, &(unsigned short)l, root, readed_bits);
        return tree_search(hdt, code << 1, &(unsigned short)r, root, readed_bits);
    }
}

void decode_file(char *input_fpath, char *output_fpath)
{
    FILE *f1, *f2;
    if ( (f1 = fopen("/home/vadim/CLion/ClionProjects/huffman/out.huf", "r")) == NULL ||
         (f2 = fopen("/home/vadim/CLion/ClionProjects/huffman/in1.txt", "w")) == NULL  )
        //if ( (f1 = fopen(input_fpath, "r")) == NULL )
        //if ( (f2 = fopen(output_fpath, "w")) == NULL )
    {
        printf("Impossible to open file %s.\n", "/home/vadim/CLion/ClionProjects/huffman/out.huf");
        //printf("Impossible to open file %s or %s. Check paths.\n", input_fpath, output_fpath);
        exit(1);
    }
    else
    {
        huffman_decode_tree *hdt = make_decode_tree(f1, hdt);
        int c = EOF;
        unsigned short root = hdt->nodes_number - 1;
        unsigned short position = root;
        while ( (c = fgetc(f1)) != EOF)
        {
            char ch = tree_search(hdt, (unsigned char)c, &position, root, 0);
            if(position != root) continue;
            else fprintf(f2, "%c", ch);
        }
    }
}
