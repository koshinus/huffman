//
// Created by vadim on 16.03.17.
//

#include "decode.h"

char tree_search(huffman_decode_tree *hdt, unsigned char code, short *pos, unsigned short root, char readed_bits)
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

        if(code << 7 == 0) return tree_search(hdt, code << 1, &l, root, readed_bits);
        else return tree_search(hdt, code << 1, &r, root, readed_bits);
    }
}