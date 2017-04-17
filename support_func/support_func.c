//
// Created by vadim on 08.02.17.
//

#include "support_func.h"

char * to_binary(uint64_t code, uint16_t code_size)
{
    char *s = (char *)malloc(code_size + 1); s[code_size] = '\0';
    for(int i = 0; i < code_size; i++)
        s[code_size - i - 1] = (char) (((code >> i) & 1) + 48);
    return s;
}

void get_frequency_table(FILE *f, uint64_t *buf)
{
    memset(buf, 0, sizeof(uint64_t)*BUFFER_SIZE);
    int c;
    while ((c = fgetc(f)) != EOF) buf[(unsigned char)c]++;
}

uint16_t count_symbols_with_not_null_frequency(uint64_t *buf)
{
    uint16_t count = 0;
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