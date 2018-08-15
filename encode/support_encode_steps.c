//
// Created by vadim on 16.03.17.
//

#include "encode.h"

void get_huffman_codes_step(huffman_encode_tree *het, int16_t parent, int16_t child, int8_t right)
{
    /*TODO: document all byte operations!!!!!
     * */
    het->tree[child].code_size = het->tree[parent].code_size + (short)1;
    uint16_t parent_bytes = (uint16_t) (het->tree[parent].code_size/LL_SIZE);
    uint16_t  child_bytes = (uint16_t) (het->tree[ child].code_size/LL_SIZE);
    if(child_bytes > parent_bytes)
    {
        het->tree[child].code = (uint64_t *)calloc(child_bytes, sizeof(uint64_t));
        for(int i = 0; i < parent_bytes; i++)
            het->tree[child].code[i] = het->tree[parent].code[i];
        if(right) het->tree[child].code[parent_bytes] = 1;
    }
    else
    {
        uint16_t last_byte = (uint16_t)LIMIT(parent_bytes);
        for(int i = 0; i < last_byte; i++)
            het->tree[child].code[i] = het->tree[parent].code[i];
        het->tree[child].code[last_byte] = het->tree[parent].code[last_byte] << 1;
        if(right) het->tree[child].code[last_byte]++;
    }
}

huffman_encode_node * get_huffman_node_by_symbol(huffman_encode_tree *het, unsigned char symbol)
{
    for(int i = 0; i < het->nodes_number/2 + 1; i++)
        if(het->tree[i].symbol == symbol)
            return &het->tree[i];
    return NULL;
}

int16_t get_minimum(huffman_encode_tree *het)
{
    int16_t i = 0, min = 0;
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

void write_tree_to_file(FILE *f, huffman_encode_tree *het)
{
    out_bytes_count = sizeof(het->nodes_number);
    int8_t decode_node_size = sizeof(het->tree[0].symbol) + sizeof(het->tree[0].left) + sizeof(het->tree[0].right);
    out_buffer[0] = ((int8_t *)&het->nodes_number)[0];
    out_buffer[1] = ((int8_t *)&het->nodes_number)[1];
    for(uint16_t i = 0; i < het->nodes_number; i++)
    {
        huffman_encode_node * node = &het->tree[i];
        out_buffer[out_bytes_count] = node->symbol;
        out_buffer[out_bytes_count + 1] = ((int8_t *)&node->left)[0];
        out_buffer[out_bytes_count + 2] = ((int8_t *)&node->left)[1];
        out_buffer[out_bytes_count + 3] = ((int8_t *)&node->right)[0];
        out_buffer[out_bytes_count + 4] = ((int8_t *)&node->right)[1];
        out_bytes_count += decode_node_size;
        if(OUT_BUFFER_SIZE - out_bytes_count < decode_node_size)
        {
            fwrite(out_buffer, sizeof(int8_t), out_bytes_count, f);
            out_bytes_count = 0;
        }
    }
}

void encode(FILE *fin, huffman_encode_tree *het, const char *fpath)
{
    char *out_path = concat(fpath, ".huf");
    FILE *fout;
    if ( (fout = fopen(out_path, "w")) == NULL )
    {
        printf("Impossible to open file %s.\n", out_path);
        free(out_path);
        exit(1);
    }
    else
    {
        free(out_path);
        write_tree_to_file(fout, het);
        rewind(fin);
        char ch = 0;
        uint16_t bits_write_in_ch = 0;
        do
        {
            in_bytes_count = fread(in_buffer, sizeof(int8_t), IN_BUFFER_SIZE, fin);
            for(uint64_t k = 0; k < in_bytes_count; k++)
            {
                huffman_encode_node *node = get_huffman_node_by_symbol(het, (unsigned char)in_buffer[k]);
                if (!node)
                {
                    printf("Something terrible is happening: cannot find node of tree! File will not be encoded completely!\n");
                    return;
                }
                uint16_t limit = (uint16_t) LIMIT(node->code_size/LL_SIZE);
                for(int i = 0; i < limit; i++)
                    for(int j = 0; j < LL_SIZE; j++)
                        encode_step(fout, node, &bits_write_in_ch, &ch, i, j, 0);
                for (int i = 0; i < node->code_size % LL_SIZE; i++)
                    encode_step(fout, node, &bits_write_in_ch, &ch, limit, i, 1);
            }
        }
        while(in_bytes_count == IN_BUFFER_SIZE);
        if(bits_write_in_ch != 0)
        {
            out_buffer[out_bytes_count++] = ch << (BYTE_SIZE - bits_write_in_ch);
            fwrite(out_buffer, sizeof(int8_t), out_bytes_count, fout);
        }
        fclose(fout);
    }
}

void encode_step(FILE *f, huffman_encode_node *node, uint16_t *bits_write_in_ch,
                 char *ch, int node_code_num, int count, int8_t last)
{
    if(*bits_write_in_ch == BYTE_SIZE)
    {
        out_buffer[out_bytes_count++] = *ch;
        if(out_bytes_count == OUT_BUFFER_SIZE)
        {
            fwrite(out_buffer, sizeof(int8_t), out_bytes_count, f);
            out_bytes_count = 0;
        }
        *ch = 0;
        *bits_write_in_ch = 0;
    }
    (*ch) <<= 1;
    (*bits_write_in_ch)++;
    if(last) (*ch) |= (node->code[node_code_num] >> (node->code_size % LL_SIZE - count - 1)) & 1;
    else (*ch) |= (node->code[node_code_num] >> (LL_SIZE - count - 1)) & 1;
}
