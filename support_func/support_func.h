//
// Created by vadim on 08.02.17.
//

#ifndef HUFFMAN_DEBUG_H
#define HUFFMAN_DEBUG_H

#include<stdio.h>
#include<stddef.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>

#define         BYTE_SIZE 8
#define           LL_SIZE 64
#define       BUFFER_SIZE 256

/*
#define         TEXT_READ 11
#define       BINARY_READ 12
#define        TEXT_WRITE 21
#define      BINARY_WRITE 22
#define   TEXT_READ_WRITE 13
#define BINARY_READ_WRITE 14
#define      TEXT_REWRITE 23
#define    BINARY_REWRITE 24
*/

#define LIMIT(X) ((X > 0)? (X-1): (0))

/*
char file_info[2][4] = {{"r", "rb", "r+", "rb+"},
                        {"w", "wb", "w+", "wb+"}};
*/

char * to_binary(char *, unsigned long long, unsigned short);
void get_frequency_table(FILE *, unsigned long long *);
unsigned short count_symbols_with_not_null_frequency(unsigned long long *);
char * get_dir_path_from_full_path(const char *);
char * concat(const char *, const char *);

#endif //HUFFMAN_DEBUG_H
