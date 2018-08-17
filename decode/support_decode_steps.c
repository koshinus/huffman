//
// Created by vadim on 16.03.17.
//

#include "decode.h"

void decode(huffman_decode_tree *hdt, FILE *fin, FILE *fout)
{
    int c = 1;
    short root = hdt->nodes_number - 1, position = root;
    int8_t bits_read = 0;
    out_bytes_count = 0;
    uint64_t i = position_in_buffer(hdt->nodes_number, sizeof(huffman_decode_node));
    while(1)
    {
        if (bits_read == 0)
        {
            c = in_buffer[i++];
            if (i == in_bytes_count)
            {
                if (in_bytes_count != IN_BUFFER_SIZE)
                {
                    //printf("%d\n", hdt->tree[position].left);
                    position = tree_search(hdt, (unsigned char *)(&in_buffer[i]), position, root, &bits_read);
                    //printf("%d\n", hdt->tree[position].left);
                    out_buffer[out_bytes_count++] = hdt->tree[position].symbol;
                    // Need to avoid "\ No newline at end of file" when compare with "diff" utility
                    //out_buffer[out_bytes_count++] = '\n';
                    fwrite(out_buffer, sizeof(int8_t), out_bytes_count, fout);
                    return;
                }
                in_bytes_count = fread(in_buffer, sizeof(int8_t), IN_BUFFER_SIZE, fin);
                i = 0;
            }
        }
        if ((i == in_bytes_count - 1 || i == in_bytes_count - 2) && in_bytes_count != IN_BUFFER_SIZE)
            printf("%d\n", in_buffer[i]);
        position = tree_search(hdt, (unsigned char *)(&c), position, root, &bits_read);
        if(hdt->tree[position].left == -1)
        {
            out_buffer[out_bytes_count++] = hdt->tree[position].symbol;
            if (out_bytes_count == OUT_BUFFER_SIZE)
            {
                fwrite(out_buffer, sizeof(int8_t), OUT_BUFFER_SIZE, fout);
                out_bytes_count = 0;
            }
            position = root;
        }
    }
}

int16_t tree_search(huffman_decode_tree *hdt, unsigned char *code, int16_t pos, int16_t root, int8_t *bits_read)
{
    int16_t l = hdt->tree[pos].left, r = hdt->tree[pos].right, descendent = r;
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
            descendent = l;
        *code <<= 1;
        return tree_search(hdt, code, descendent, root, bits_read);
    }
}
