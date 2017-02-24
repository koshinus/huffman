#include "encode_tree.h"

int main()
{
    const char *filename;
    FILE *fin;
    printf("Enter file name:\n");
    scanf("%ms", &filename);
    if ( (fin = fopen("/home/vadim/CLion/ClionProjects/huffman/test_file.txt", "r")) == NULL )
    {
        printf("Impossible to open file %s.\n", filename);
        exit(1);
    }
    encode_file(fin);
    fclose(fin);
    return 0;
}