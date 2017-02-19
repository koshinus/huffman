#include "huffman_tree.h"

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

	//Getting frequency table from text or binary file
	unsigned long long buffer[BUFFER_SIZE];
    get_frequency_table(fin, buffer);

    unsigned short symbols_with_not_null_freq = count_symbols_with_not_null_frequency(buffer);

    //https://en.wikipedia.org/wiki/Huffman_coding
    //Algorithm steps:
    huffman_encode_tree *het = make_tree(het, buffer, symbols_with_not_null_freq);			// 1
    huffman_algorithm(het, symbols_with_not_null_freq);                                     //2.1 - 2.3

	//Getting huffman codes for leafs
    get_huffman_codes_for_symbols(het, het->nodes_number-(short)1);

    //Visualize huffman tree
    make_visualization(het);

    //Encode file
    encode_file(fin, het);

    free(het);
    fclose(fin);
    return 0;
}