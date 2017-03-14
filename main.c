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
    if(!strcmp(argv[2], "-b")) is_text = 0;
    if( !strcmp(argv[2], "-info") ||
        !strcmp(argv[3], "-info")  )
        debug = 1;
    if(strcmp(argv[2], "-en")) encode_file(argv[argc - 1], is_text, debug);
    //else decode_file()

    return 0;
}