#include "encode/encode.h"
#include "decode/decode.h"
#include <omp.h>

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 5 ||
            //(strcmp(argv[1], "-en") != 0 && strcmp(argv[1], "-de") != 0))
            (strcmp(argv[1], "-en") && strcmp(argv[1], "-de")))
    {
        printf("Incorrect input! Follow the example!");
        exit(1);
    }
    int8_t info = 0, is_text = 1;
    for (int i = 2; i < argc - 1; i++)
    {
        if(!strcmp(argv[i], "-b")) is_text = 0;
        if(!strcmp(argv[i], "-info")) info = 1;
    }
    //if(strcmp(argv[1], "-en") == 0)
    if(!strcmp(argv[1], "-en"))
    {
        double start = omp_get_wtime();
        encode_file(argv[argc - 1], is_text, info);
        double end = omp_get_wtime();
        printf("File was encoded in %lf ms time.\n", end - start);
    }
    else
    {
        double start = omp_get_wtime();
        decode_file(argv[argc - 1], is_text);
        double end = omp_get_wtime();
        printf("File was decoded in %lf ms time.\n", end - start);
    }

    return 0;
}
