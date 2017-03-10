#include "encode_tree.h"
#include "decode_tree.h"

int main(int argc, char *argv[])
{
    if(argc < 4)
    {
        printf("Incorrect input! Follow the example!");
        exit(1);
    }
    char debug = 0, is_text = 1;
    if(strcmp(argv[2], "-b") == 0) is_text = 0;
    if( !strcmp(argv[2], "-info") ||
        !strcmp(argv[3], "-info")  )
        debug = 1;

    /*
    const char *filename;
    FILE *fin;
    printf("Enter file name:\n");
    scanf("%ms", &filename);
    if ( (fin = fopen("/home/vadim/CLion/ClionProjects/huffman/in.txt", "r")) == NULL )
    {
        printf("Impossible to open file %s.\n", filename);
        exit(1);
    }
    encode_file(fin);
    //decode_file("d", "d");
    fclose(fin);
    */
    return 0;
}