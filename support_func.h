//
// Created by vadim on 08.02.17.
//

#ifndef HUFFMAN_DEBUG_H
#define HUFFMAN_DEBUG_H

char * to_binary(char *, unsigned long long, unsigned short);
unsigned short get_limit(unsigned short);
void get_frequency_table(FILE *, unsigned long long *);
unsigned short count_symbols_with_not_null_frequency(unsigned long long *);

#endif //HUFFMAN_DEBUG_H
