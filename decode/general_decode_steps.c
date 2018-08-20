//
// Created by vadim on 16.03.17.
//

#include "decode.h"

void decode_file(const char *fpath, int8_t text)
{
    FILE *fin, *fout;
    char *dir_path = get_dir_path_from_full_path(fpath);
    char *decode_path;
    if(text)
    {
        decode_path = concat(dir_path, "out.txt");
        fout = fopen(decode_path, "w");
    }
    else
    {
        decode_path = concat(dir_path, "out.x");
        fout = fopen(decode_path, "wb");
    }
    if ( (fin = fopen(fpath, "r")) == NULL
         || fout == NULL  )
    {
        printf("Impossible to open file %s or %s. Check paths.\n", fpath, decode_path);
        free(dir_path); free(decode_path);
        exit(1);
    }
    else
    {
        free(dir_path); free(decode_path);
        huffman_decode_tree *hdt = make_decode_tree(fin);
        decode(hdt, fin, fout, text);
        free(hdt);
        fclose(fin); fclose(fout);
    }
}
