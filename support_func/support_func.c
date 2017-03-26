//
// Created by vadim on 08.02.17.
//

#include "support_func.h"

char * to_binary(unsigned long long code, unsigned short code_size)
{
    char *s = (char *)malloc(code_size + 1); s[code_size] = '\0';
    for(int i = 0; i < code_size; i++)
        s[code_size - i - 1] = (char)((code >> i) & 1) + 48;
    return s;
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

char * concat(const char *src1, const char *src2)
{
    char *res;
    if(src1 == NULL && src2 == NULL) return NULL;
    else if(src1 != NULL && src2 != NULL)
    {
        res = (char *) malloc(strlen(src1) + strlen(src2) + 1);
        strcpy(res, src1);
        strcat(res, src2);
    }
    else
    {
        const char *src = (src1 != NULL)? src1: src2;
        res = (char *) malloc(strlen(src));
        strcpy(res, src);
    }
    /*
    if(src1 != NULL && src2 == NULL)
    {
        res = (char *) malloc(strlen(src1));
        strcpy(res, src1);
    }
    else if(src1 == NULL && src2 != NULL)
    {
        res = (char *) malloc(strlen(src2));
        strcpy(res, src2);
    }
    else
    {
        res = (char *) malloc(strlen(src1) + strlen(src2) + 1);
        strcpy(res, src1);
        strcat(res, src2);
    }
    */
    return res;
}

char * get_dir_path_from_full_path(const char *full_path)
{
    char *dir_path = NULL;
    char *last_slash = NULL;
    last_slash = strrchr(full_path, '/');
    if(last_slash == NULL)
        return dir_path;
    dir_path = strndup(full_path, last_slash - full_path + 1);
    return dir_path;
}

void test()
{
    uint16_t x = 8, y = 6;
    uint16_t z = x/y;
    uint16_t w = (uint16_t) LIMIT((x*y)/LL_SIZE);
}