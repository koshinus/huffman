//
// Created by vadim on 16.03.17.
//

#include "encode.h"

int encode_file(const char *fpath, int8_t text, int8_t debug)
{
    FILE *fin;
    if(text)
        fin = fopen(fpath, "r");
    else
        fin = fopen(fpath, "rb");
    if(fin == NULL)
    {
        printf("Impossible to open file %s.\n", fpath);
        exit(1);
    }
    //Getting frequency table from text or binary file
    uint64_t buffer[BUFFER_SIZE];
    get_frequency_table(fin, buffer);

    uint16_t symbols_with_not_null_freq = count_symbols_with_not_null_frequency(buffer);

    //https://en.wikipedia.org/wiki/Huffman_coding
    //Algorithm steps:
    huffman_encode_tree *het = make_encode_tree(buffer, symbols_with_not_null_freq);		//1
    huffman_algorithm(het, symbols_with_not_null_freq);                                     //2.1 - 2.3

    //Getting huffman codes for leafs
    get_huffman_codes_for_symbols(het, het->nodes_number-1);

    //Encode file
    encode(fin, het, fpath);

    if(debug)
    {
        //rewind(fin);
        write_debug_info(fin, het, fpath, buffer, symbols_with_not_null_freq);
    }

    free(het);
    fclose(fin);

    return 0;
}

void huffman_algorithm(huffman_encode_tree *het, uint16_t count)
{
    while(2*count - het->nodes_number > 1)			                        // 2
    {
        int16_t left, right;
        left  = get_minimum(het);											// 2.1
        het->tree[ left].free_and_is_leaf |= 2;								// 2.1
        right = get_minimum(het);											// 2.1
        het->tree[right].free_and_is_leaf |= 2;								// 2.1
        huffman_encode_node node
                = make_encode_node(het->tree[ left].frequency +				// 2.2
                                   het->tree[right].frequency,	            // 2.2
                                   0, left, right, 0, '-');					// 2.2
        het->tree[het->nodes_number] = node;					    		// 2.3
        het->nodes_number++;
    }
}

void get_huffman_codes_for_symbols(huffman_encode_tree *het, int16_t parent)
{
    int16_t l = het->tree[parent].left, r = het->tree[parent].right;
    if(l == -1) return;
    else
    {
        get_huffman_codes_step(het, parent, l, 0);
        get_huffman_codes_for_symbols(het, l);
        get_huffman_codes_step(het, parent, r, 1);
        get_huffman_codes_for_symbols(het, r);
    }
}
