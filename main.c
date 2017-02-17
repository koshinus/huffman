#include"suppory_func.h"

int main()
{
    const char *filename;
    FILE *fin;
    printf("Enter file name:\n");
    scanf("%ms", &filename);
    if ( (fin = fopen("/home/vadim/CLion/ClionProjects/huffman/in.txt", "r")) == NULL )
    {
        printf("Impossible to open file %s.\n", filename);
        exit(1);
    }

	//Getting frequency table from text or binary file
	unsigned long long buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(unsigned long long)*BUFFER_SIZE);
    int c = EOF;
    while ((c = fgetc(fin)) != EOF) buffer[(unsigned char)c]++;

    unsigned short symbols_with_not_null_freq = 0;
    for(int i = 0; i < BUFFER_SIZE; i++)
    {
        if (buffer[i] != 0) symbols_with_not_null_freq++;
        printf("%d = %c - %lli | %d\n", i, (unsigned char)i, buffer[i], symbols_with_not_null_freq);
    }
    //printf("%d", symbols_with_not_null_freq);
    //https://en.wikipedia.org/wiki/Huffman_coding
    //Algorithm steps:
    huffman_encode_tree *het = make_tree(het, buffer, symbols_with_not_null_freq);			// 1
    for(int i = 0; i < het->nodes_number; i++)
    {
        printf("%d = %c - %li\n", (int)het->tree[i].symbol, het->tree[i].symbol, het->tree[i].frequency);
    }
    //qsort(het->tree, het->nodes_number, sizeof(huffman_encode_node), cmp);
	//for(int i = 0; i < het->nodes_number; i++)
        //printf("%c: %ld\n", het->tree[i].symbol, het->tree[i].frequency);

    while(2*symbols_with_not_null_freq - het->nodes_number > 1)			    // 2
    {
        short left = 0, right = 0;  					            	
        left  = get_minimum(het);											// 2.1
        het->tree[ left].free_and_is_leaf |= 2;								// 2.1
        right = get_minimum(het);											// 2.1
        het->tree[right].free_and_is_leaf |= 2;								// 2.1
        huffman_encode_node node
                = make_node(het->tree[ left].frequency+					    // 2.2
                            het->tree[right].frequency,	                    // 2.2
                            0, left, right, 0, '-');						// 2.2
        het->tree[het->nodes_number] = node;					    		// 2.3
        het->nodes_number++;
    }
	
	//Printing nodes frequency
    /*
    for(int i = 0; i < het->nodes_number; i++)
        printf("%d: {%d, %d}->%ld\n", i, het->tree[i].left, het->tree[i].right, het->tree[i].frequency);
	*/

	//Getting huffman codes for leafs
    get_huffman_codes_for_symbols(het, het->nodes_number-(short)1);

    //Visualize huffman tree
    make_visualization(het);

	//Printing huffman codes in binary view
    /*
    for(int i = 0; i < het->nodes_number/2 + 1; i++)
    {
        unsigned short elem_amount = het->tree[i].code_size/LONG_LONG_SIZE;
        char s1[LONG_LONG_SIZE + 1], s2[het->tree[i].code_size%LONG_LONG_SIZE + 1];
        s1[LONG_LONG_SIZE] = '\0'; s2[het->tree[i].code_size%LONG_LONG_SIZE] = '\0';
        for(int j = 0; j < elem_amount; j++)
            printf("(%d): %s\n", het->tree[i].code_size, to_binary(s1, het->tree[i].code[j], elem_amount));
        printf("(%d): %s\n", het->tree[i].code_size,
               to_binary(s2, het->tree[i].code[elem_amount], het->tree[i].code_size%LONG_LONG_SIZE));
    }
    */

    FILE *fout;
    if ( (fout = fopen("/home/vadim/CLion/ClionProjects/huffman/out.huf", "w")) == NULL )
    {
        printf("Impossible to open file %s.\n", "/home/vadim/CLion/ClionProjects/huffman/out.huf");
        exit(1);
    }
    else
    {
        for(int i = 0; i < het->nodes_number/2 + 1; i++)
        {
            unsigned short elem_amount = het->tree[i].code_size/LONG_LONG_SIZE;
            char s1[LONG_LONG_SIZE + 1], s2[het->tree[i].code_size%LONG_LONG_SIZE + 1];
            s1[LONG_LONG_SIZE] = '\0'; s2[het->tree[i].code_size%LONG_LONG_SIZE] = '\0';
            for(int j = 0; j < elem_amount; j++)
                fprintf(fout, "%c%s", het->tree[i].symbol, to_binary(s1, het->tree[i].code[j], elem_amount));
            fprintf(fout, "%c%s", het->tree[i].symbol,
                   to_binary(s2, het->tree[i].code[elem_amount], het->tree[i].code_size%LONG_LONG_SIZE));
            //printf("%d....OK\n", i);
        }
        rewind(fin);
        c = EOF;
        //printf("OK");
        char ch = 0;
        unsigned short bits_write_in_ch = 0;
        while( (c = fgetc(fin)) != EOF )
        {
            //printf("%c....", c);
            huffman_encode_node *node = get_huffman_node_by_symbol(het, (unsigned char)c);
            int limit = (node->code_size/LONG_LONG_SIZE > 0)? node->code_size/LONG_LONG_SIZE - 1: 0;
            for(int i = 0; i < limit; i++)
                for(int j = 0; j < LONG_LONG_SIZE; j++)
                {
                    if(bits_write_in_ch == 8)
                    {
                        fprintf(fout, "%c", ch);
                        ch = 0;
                        bits_write_in_ch = 0;
                    }
                    ch <<= 1;
                    ch |= (node->code[i] >> (LONG_LONG_SIZE - i - 1)) & 1;
                    bits_write_in_ch++;
                }
            for (int i = 0; i < node->code_size%LONG_LONG_SIZE; i++)
            {
                if(bits_write_in_ch == 8)
                {
                    fprintf(fout, "%c", ch);
                    ch = 0;
                    bits_write_in_ch = 0;
                }
                ch <<= 1;
                ch |= (node->code[limit] >> (node->code_size%LONG_LONG_SIZE - i - 1)) & 1;
                bits_write_in_ch++;
            }
            //printf("%d%d%c\n", 0, 1, '<');
        }
        fclose(fout);
    }
    free(het);
    fclose(fin);
    return 0;
}