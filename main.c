#include "encode/encode.h"
#include "decode/decode.h"

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 5 ||
            //(strcmp(argv[1], "-en") != 0 && strcmp(argv[1], "-de") != 0))
            (strcmp(argv[1], "-en") && strcmp(argv[1], "-de")))
    {
        printf("Incorrect input! Follow the example!");
        exit(1);
    }
    printf("%d\n", 1);
    char info = 0, is_text = 1;
    for (int i = 2; i < argc - 1; i++)
    {
        if(strcmp(argv[i], "-b")) is_text = 0;
        if(strcmp(argv[i], "-info")) info = 1;
    }
    //if(strcmp(argv[1], "-en") == 0)
    if(!strcmp(argv[1], "-en"))
        encode_file(argv[argc - 1], is_text, info);
    else decode_file(argv[argc - 1], is_text);

    return 0;
}
