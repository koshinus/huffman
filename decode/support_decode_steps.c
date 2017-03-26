//
// Created by vadim on 16.03.17.
//

#include "decode.h"

void decode(huffman_decode_tree *hdt, FILE *fin, FILE *fout)
{
    /*TODO: add removing of last character from file "fout"
     * */
    int c = 1;
    short root = hdt->nodes_number - 1, position = root;
    char bits_read = 0;
    unsigned char ch = (unsigned char)c;
    while ( 1 )
    {
        if(bits_read == 0)
        {
            c = fgetc(fin);
            ch = (unsigned char)c;
        }
        if(c == EOF) break;
        position = tree_search(hdt, &ch, position, root, &bits_read);
        if(hdt->tree[position].left == -1)
        {
            fprintf(fout, "%c", hdt->tree[position].symbol);
            position = root;
        }
    }
}

short tree_search(huffman_decode_tree *hdt, unsigned char *code, short pos, short root, char *bits_read)
{
    short l = hdt->tree[pos].left, r = hdt->tree[pos].right;
    if(l == -1) return pos;
    else if((*bits_read) == 8)
    {
        *bits_read = 0;
        return pos;
    }
    else
    {
        (*bits_read)++;
        if((*code & (1 << 7)) == 0)
        {
            *code <<= 1;
            return tree_search(hdt, code, l, root, bits_read);
        }
        else
        {
            *code <<= 1;
            return tree_search(hdt, code, r, root, bits_read);
        }
    }
}
