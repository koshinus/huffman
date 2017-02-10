#include"suppory_func.h"

char * to_binary(char *str, char code, unsigned long code_size)//FILE *f, char code, unsigned long code_size)
{
    //fprintf(f, "%c", code);
    for(int i = 0; i < code_size; i++)
    {
        str[i] = (char) ((code >> i) & 1) + 48;
    }
	return str;
}

char * to_binary1(char *str, char code, unsigned long code_size)//FILE *f, char code, unsigned long code_size)
{
    //fprintf(f, "%c", code);
    for(int i = 0; i < code_size; i++)
    {
        str[7 - i] = (char) ((code >> i) & 1) + 48;
    }
    return str;
}

int main()
{
    const char *filename;
    FILE *f;
    printf("Enter file name:\n");
    scanf("%ms", &filename);
    if ( (f = fopen(filename, "r")) == NULL )
    {
        printf("Impossible to open file %s.\n", filename);
        exit(1);
    }

	//Getting frequency table from text or binary file
	unsigned long long buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(unsigned long long)*BUFFER_SIZE);
    int c = EOF;
    while ((c = fgetc(f)) != EOF) buffer[(unsigned char)c]++;

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

    while(2*symbols_with_not_null_freq - het->nodes_number > 1)							// 2
    {
        short left = 0, right = 0;  					            	
        left  = get_minimum(het);											// 2.1
        het->tree[ left].free_and_is_leaf |= 2;								// 2.1
        right = get_minimum(het);											// 2.1
        het->tree[right].free_and_is_leaf |= 2;								// 2.1
        huffman_encode_node node
                = make_node(het->tree[ left].frequency+						// 2.2
                            het->tree[right].frequency,						// 2.2
                            0, left, right, 0, 0, '-');						// 2.2
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
		char s1[9] = {'-', '-', '-', '-', '-', '-', '-', '-', '\0'};
        unsigned short code_size = het->tree[i].code_size;
        unsigned char code = het->tree[i].code;
        char *s = (char *)malloc(code_size + (short)1);
        s[code_size] = '\0';
        printf("%c(%d): %s\n", code, code_size, to_binary(s, code, code_size));
        printf("%c(%d): %s\n", code, code_size, to_binary1(s1, code, code_size));
        free(s);
	}
    */
    free(het);
    fclose(f);
	
    return 0;
}