#include "encode/encode.h"
#include "decode/decode.h"

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Incorrect input! Follow the example!");
        exit(1);
    }
    char info = 0, is_text = 1;
    if(!strcmp(argv[2], "-b")) is_text = 0;
    if( !strcmp(argv[2], "-info") ||
        !strcmp(argv[3], "-info")  )
        info = 1;
    if(strcmp(argv[2], "-en")) encode_file(argv[argc - 1], is_text, info);
    else decode_file(argv[argc - 1], is_text);

    return 0;
}