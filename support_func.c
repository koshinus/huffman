//
// Created by vadim on 08.02.17.
//

#include "huffman_tree.h"

char * to_binary(char *s, unsigned long long code, unsigned short code_size)
{
    for(int i = 0; i < code_size; i++)
        s[code_size - i - 1] = (char)((code >> i) & 1) + 48;
    return s;
}

unsigned short get_limit(unsigned short num)
{
    return (num > 0)? num - 1: 0;
}

void get_frequency_table(FILE *f, unsigned long long *buf)
{
    memset(buf, 0, sizeof(unsigned long long)*BUFFER_SIZE);
    int c = EOF;
    while ((c = fgetc(f)) != EOF) buf[(unsigned char)c]++;
}

unsigned short count_symbols_with_not_null_frequency(unsigned long long *buf)
{
    unsigned short count = 0;
    for(int i = 0; i < BUFFER_SIZE; i++)
        if (buf[i] != 0) count++;
    return count;
}