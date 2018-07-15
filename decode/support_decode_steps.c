//
// Created by vadim on 16.03.17.
//

#include "decode.h"
/*
void decode(huffman_decode_tree *hdt, FILE *fin, FILE *fout)
{
    /*TODO: add removing of last character from file "fout"
     *
    int32_t c = fgetc(fin);
    if(c == EOF) return;
    int16_t root = hdt->nodes_number - 1, position = root;
    int8_t bits_read = 0;
    unsigned char cur = (unsigned char)c, prev;
    while ( 1 )
    {
        if(bits_read == 0)
        {
            prev = cur;
            c = fgetc(fin);
            if (c == EOF)
            {
                position = tree_search(hdt, &prev, position, root, &bits_read);
                fprintf(fout, "%c", hdt->tree[position].symbol);
                break;
            }
            cur = (unsigned char)c;
        }
        //if(c == EOF) break;
        position = tree_search(hdt, &prev, position, root, &bits_read);
        if(hdt->tree[position].left == -1)
        {
            fprintf(fout, "%c", hdt->tree[position].symbol);
            position = root;
        }
    }
}
//*/
///*
void decode(huffman_decode_tree *hdt, FILE *fin, FILE *fout)
{
    //uint64_t written_bytes = 0;
    int c = 1;
    short root = hdt->nodes_number - 1, position = root;
    int8_t bits_read = 0;
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
            //written_bytes++;
            fprintf(fout, "%c", hdt->tree[position].symbol);
            position = root;
        }
    }
}
//*/
int16_t tree_search(huffman_decode_tree *hdt, unsigned char *code, int16_t pos, int16_t root, int8_t *bits_read)
{
    int16_t l = hdt->tree[pos].left, r = hdt->tree[pos].right;
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
